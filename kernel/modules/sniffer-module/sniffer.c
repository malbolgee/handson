#include <linux/module.h>
#include <linux/usb.h>
#include <linux/slab.h>

MODULE_AUTHOR("Grupo-2 <devtitans@icomp.ufam.edu.br>");
MODULE_DESCRIPTION("Driver de acesso ao Sniffer (ESP32 com Chip Serial CP2102");
MODULE_LICENSE("GPL");

#define MAX_RECV_LINE 100 /** The maximum length of the USB response.  */

static int usb_probe(struct usb_interface *ifce, const struct usb_device_id *id);
static void usb_disconnect(struct usb_interface *ifce);
static int usb_send_cmd(char *cmd);
static ssize_t attr_show(struct kobject *sys_obj, struct kobj_attribute *attr, char *buff);

static char recv_line[MAX_RECV_LINE];
static struct usb_device *sniffer_device;
static uint usb_in, usb_out;
static char *usb_in_buffer, *usb_out_buffer;
static int usb_max_size;

static struct kobj_attribute rssi_attribute = __ATTR(rssi, S_IRUGO | S_IWUSR, attr_show, NULL);
static struct attribute *attrs[] = {&rssi_attribute.attr, NULL};
static struct attribute_group attr_group = {.attrs = attrs};
static struct kobject *sys_obj;

/** Registers the CP2102 (ESP32's USB-Serial chip) on the kernel's UBS-Core */
#define VENDOR_ID 0x10C4  /** VendorID  do CP2102 */
#define PRODUCT_ID 0xEA60 /** ProductID do CP2102 */
static const struct usb_device_id id_table[] = {{USB_DEVICE(VENDOR_ID, PRODUCT_ID)}, {}};
MODULE_DEVICE_TABLE(usb, id_table);

/** Registers the sniffer drive on the kernel */
static struct usb_driver sniffer_driver = {
    .name = "sniffer", // Driver's name
    .probe = usb_probe,
    .disconnect = usb_disconnect,
    .id_table = id_table, // Table with the product's VendorID and ProductID
};

module_usb_driver(sniffer_driver);

/**
 * @brief This function executes when the device is connected to the USB port
 *
 * @param interface is the usb interface reference. The communication channel between
 *          the USB driver and this module.
 * @param id is the identification of the USB device we are talking to.
 */
static int usb_probe(struct usb_interface *interface, const struct usb_device_id *id)
{
    struct usb_endpoint_descriptor *usb_endpoint_in, *usb_endpoint_out;

    printk(KERN_INFO "Sniffer: Device connected ...\n");

    sys_obj = kobject_create_and_add("sniffer", kernel_kobj);
    sysfs_create_group(sys_obj, &attr_group);

    sniffer_device = interface_to_usbdev(interface);
    usb_find_common_endpoints(interface->cur_altsetting, &usb_endpoint_in, &usb_endpoint_out, NULL, NULL);
    usb_max_size = usb_endpoint_maxp(usb_endpoint_in);
    usb_in = usb_endpoint_in->bEndpointAddress;
    usb_out = usb_endpoint_out->bEndpointAddress;
    usb_in_buffer = kmalloc(usb_max_size, GFP_KERNEL);
    usb_out_buffer = kmalloc(usb_max_size, GFP_KERNEL);

    return 0;
}

/**
 * @brief This function is executed when  the USB device is disconnected from the UBS entry.
 *
 * If the sys object is valid (i.e, not NULL), then the files created by this module
 * will be removed and the USB buffers will dealoceted.
 *
 * @param interface Is the USB interface used by this module.
 */
static void usb_disconnect(struct usb_interface *interface)
{
    printk(KERN_INFO "Sniffer: Device disconnected.\n");
    if (sys_obj)
        kobject_put(sys_obj);
    kfree(usb_in_buffer);
    kfree(usb_out_buffer);
}

/**
 * @brief This function sends a command via USB and waits for a response.
 *
 * @param cmd It's a string representing the command that is to be recognized and treated accordingly.
 *
 * @return returns an integer representing the RSSI value captured by the esp32 board.
 */
static int usb_send_cmd(char *cmd)
{
    int recv_size = 0;
    int ret, actual_size, i;
    int retries = 10;
    char resp_expected[MAX_RECV_LINE];
    char *resp_pos;
    long resp_number = -1;

    printk(KERN_INFO "Sniffer: Sending command: %s\n", cmd);

    sprintf(usb_out_buffer, "%s\n", cmd);

    ret = usb_bulk_msg(sniffer_device, usb_sndbulkpipe(sniffer_device, usb_out), usb_out_buffer, strlen(usb_out_buffer), &actual_size, 1000 * HZ);
    if (ret)
    {
        printk(KERN_ERR "Sniffer: Error on code %d trying to sending the command!\n", ret);
        return -1;
    }

    sprintf(resp_expected, "RES %s", cmd);

    while (retries > 0)
    {
        ret = usb_bulk_msg(sniffer_device, usb_rcvbulkpipe(sniffer_device, usb_in), usb_in_buffer, min(usb_max_size, MAX_RECV_LINE), &actual_size, HZ * 1000);
        if (ret)
        {
            printk(KERN_ERR "Sniffer: Error on reading data from USB (attempt %d). Code: %d\n", ret, retries--);
            continue;
        }

        for (i = 0; i < actual_size; i++)
        {

            if (usb_in_buffer[i] == '\n')
            {
                recv_line[recv_size] = '\0';
                printk(KERN_INFO "Sniffer: A line received: '%s'\n", recv_line);

                if (!strncmp(recv_line, resp_expected, strlen(resp_expected)))
                {
                    printk(KERN_INFO "Sniffer: The line is the answer for %s! Processing ...\n", cmd);
                    resp_pos = &recv_line[strlen(resp_expected) + 1];
                    kstrtol(resp_pos, 10, &resp_number);
                    return resp_number;
                }
                else
                {
                    printk(KERN_INFO "Sniffer: It is not the answer for %s! Attempt %d. Next line...\n", cmd, retries--);
                    recv_size = 0;
                }
            }
            else
                recv_line[recv_size] = usb_in_buffer[i], ++recv_size;
        }
    }
    return -1;
}

/**
 * @brief This function executes when the rssi file is read.
 *
 * @param sys_obj The object (file) this module created.
 * @param attr The file attributes associated with this object
 * @param buff The pointer in which we'll put the result string usb_send_cmd returns.
 *
 * @return returns the length of the reponse.
 */
static ssize_t attr_show(struct kobject *sys_obj, struct kobj_attribute *attr, char *buff)
{
    int value = 0;
    const char *attr_name = attr->attr.name;

    printk(KERN_INFO "Sniffer: Reading %s ...\n", attr_name);

    if (!strcmp(attr_name, "rssi"))
        value = usb_send_cmd("GET_RSSI");

    sprintf(buff, "%d\n", value);
    return strlen(buff);
}