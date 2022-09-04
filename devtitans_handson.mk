$(call inherit-product, $(SRC_TARGET_DIR)/product/sdk_phone_x86_64.mk)

PRODUCT_NAME := devtitans_handson
PRODUCT_DEVICE := handson
PRODUCT_BRAND := handsonBrand
PRODUCT_MODEL := handsonModel

PRODUCT_SYSTEM_PROPERTIES += \
    ro.devtitans.name=handson

PRODUCT_PRODUCT_PROPERTIES += \
    ro.product.devtitans.version=1.0 \
    devtitans.sniffer.allow_simulated=true

PRODUCT_PACKAGES += \
    sniffer_client \
    devtitans.sniffer \
    devtitans.sniffer-service \
    sniffer_service_client \
    devtitans.sniffermanager \

BOARD_SEPOLICY_DIRS += device/devtitans/handson/sepolicy

DEVICE_FRAMEWORK_COMPATIBILITY_MATRIX_FILE := device/devtitans/handson/device_framework_matrix.xml