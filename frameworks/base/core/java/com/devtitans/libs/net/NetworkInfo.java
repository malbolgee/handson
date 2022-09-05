package com.devtitans.libs.net;

import android.util.Log;

/**
 * This class describes a NetworkInfo structure.
 *
 * <p>
 * The user can pass a string to the constructor that is returned by the Sniffer
 * HAL Service.
 * This class will parse this string and provide a parsed SSID and RSSI values.
 *
 * <p>
 * If the {@link #mNetworkInfoStringInternal} is {@code null}, then the
 * constructor
 * will throw a {@code IllegalArgumentException}.
 *
 * <p>
 * If during the parsing there are bad data, the constructor may throw a
 * {@code IllegalStateException} or a {@code NumberFormatException}.
 */
public class NetworkInfo {
    private static final String TAG = NetworkInfo.class.getSimpleName();

    private int mRSSI;
    private String mSSID;
    private String mNetworkInfoStringInternal;

    public NetworkInfo(final String networkInfo) throws IllegalStateException, NumberFormatException {
        setNetworkInfoString(networkInfo);
        parseNetworkInfo();
    }

    private void setNetworkInfoString(final String networkInfo) {
        if (networkInfo == null)
            throw new IllegalArgumentException("NetworkInfo string cannot be null");

        this.mNetworkInfoStringInternal = networkInfo;
    }

    private String getNetworkInfoString() {
        return this.mNetworkInfoStringInternal;
    }

    private void setRssi(final int rssi) {
        this.mRSSI = rssi;
    }

    public int getRssi() {
        return this.mRSSI;
    }

    private void setSsid(final String ssid) {
        this.mSSID = ssid;
    }

    public String getSsid() {
        return this.mSSID;
    }

    /**
     * This method parses the network info string received by the constructor.
     *
     * This parsing will extract the RSSI and SSID from the network info string.
     */
    private void parseNetworkInfo() throws IllegalStateException, NumberFormatException {
        Log.d(TAG, "Parsing Network info");
        final String networkInfoInternal[] = getNetworkInfoString().split(" ");

        if (networkInfoInternal.length < 2)
            throw new IllegalStateException("Insufficient Network info");

        setSsid(networkInfoInternal[0]);
        setRssi(Integer.parseInt(networkInfoInternal[1]));
        Log.d(TAG, "Done parsing Network info");
    }

    @Override
    public String toString() {
        return getSsid() + " " + getRssi();
    }
}
