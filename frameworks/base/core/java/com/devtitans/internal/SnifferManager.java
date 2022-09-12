package com.devtitans.internal;

import android.util.Log;
import android.os.ServiceManager;
import android.os.IBinder;
import android.os.RemoteException;

import com.devtitans.libs.net.SnifferNetworkInfo;

import devtitans.sniffer.NetworkInfo;
import devtitans.sniffer.SnifferDeviceState;

import devtitans.sniffer.ISniffer;

/**
 * This class is the Manager for the Sniffer HAL Service.
 *
 * <p>
 * This class serves the purpose of being a bridge between any application,
 * manager,
 * or service from the upper layer, that may want to communicate with the
 * Sniffer HAL Service.
 *
 * <p>
 * As a Singleton, there is only one instance of this class in memory. The user
 * may call the {@link #getInstance()} method to get an instance object.
 */
public class SnifferManager {
    private static final String TAG = SnifferManager.class.getSimpleName();

    public static final int DEVICE_NOT_FOUND = SnifferDeviceState.DEVICE_NOT_FOUND;
    public static final int DEVICE_IS_CONNECTED = SnifferDeviceState.DEVICE_IS_CONNECTED;
    public static final int DEVICE_IS_SIMULATED = SnifferDeviceState.DEVICE_IS_SIMULATED;

    private static final String SERVICE_NAME = "devtitans.sniffer.ISniffer/default";

    private IBinder mBinder;
    private ISniffer mService;

    private static SnifferManager mInstance;

    private SnifferManager() {
        Log.d(TAG, "Launching instance of SnifferManager");

        setBinder(ServiceManager.getService(SERVICE_NAME));

        if (getBinder() != null) {
            setService(ISniffer.Stub.asInterface(getBinder()));

            if (getService() != null)
                Log.d(TAG, "Sniffer service successfully accessed");
            else
                Log.e(TAG, "Could not access the Sniffer Service");
        } else {
            Log.e(TAG, "Could not access Binder");
        }
    }

    private void setBinder(IBinder binder) {
        this.mBinder = binder;
    }

    private IBinder getBinder() {
        return this.mBinder;
    }

    private void setService(ISniffer service) {
        this.mService = service;
    }

    private ISniffer getService() {
        return this.mService;
    }

    public static SnifferManager getInstance() {
        if (mInstance == null)
            mInstance = new SnifferManager();

        return mInstance;
    }

    public int connect() throws RemoteException {
        Log.d(TAG, "Called connect() method");
        return getService().connect();
    }

    /**
     * Returns the SnifferNetworkInfo captured by the esp32 board.
     *
     * <p>
     * This method calls for the service method and then creates a
     * {@link com.devtitans.libs.net.SnifferNetworkInfo}
     * object out of the NetworkInfo object received from the service.
     *
     * <p>
     * It is advised to call this method from a thread,
     * as it may take some time to respond.
     */
    public SnifferNetworkInfo getNetworkInfo() throws RemoteException {
        Log.d(TAG, "Called getNetworkInfo() method");

        NetworkInfo networkInfo = getService().get_network_info();

        SnifferNetworkInfo.Builder builder = new SnifferNetworkInfo.Builder();

        builder.setRssi(networkInfo.rssi)
               .setSsid(networkInfo.ssid);

        return builder.Build();
    }
}
