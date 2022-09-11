package com.devtitans.internal;

import android.util.Log;
import android.os.ServiceManager;
import android.os.IBinder;
import android.os.RemoteException;

import com.devtitans.libs.net.NetworkInfo;
import com.devtitans.libs.net.NetworkParseException;

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
     * Returns the NetworkInfo captured by the esp32 board.
     *
     * <p>
     * This method calls for the service method and then creates a
     * {@link com.devtitans.libs.net.NetworkInfo}
     * object out of the string received from the service.
     *
     * <p>
     * It is advised to call this method from a thread,
     * as it may take some time to respond.
     */
    public NetworkInfo getNetworkInfo() throws RemoteException {
        Log.d(TAG, "Called getNetworkInfo() method");

        try {
            return new NetworkInfo(getService().get_network_info());
        } catch (NumberFormatException | NetworkParseException e) {
            Log.d(TAG, e.getMessage());
            return null;
        }
    }
}
