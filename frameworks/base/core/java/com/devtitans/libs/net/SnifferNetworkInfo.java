package com.devtitans.libs.net;

import java.util.Objects;

import android.text.TextUtils;
import android.util.Log;

/**
 * The SnifferNetworkInfo will describe a NetworkInfo to the Application layer.
 *
 * <p>
 * The application layer can not see the package devtitans.sniffer.NetworkInfo.
 * So, basically, this class has been implementated to 'translate' the
 * NetworkInfo class defined in the .aidl file, to the application (and others
 * that may need).
 */
public class SnifferNetworkInfo {
    private static final String TAG = SnifferNetworkInfo.class.getSimpleName();

    private int mRssi = 1;
    private String mSsid;

    private SnifferNetworkInfo(Builder builder) {
        setRssi(builder.rssi);
        setSsid(builder.ssid);
    }

    private void setRssi(final int rssi) {
        this.mRssi = rssi;
    }

    public int getRssi() {
        return this.mRssi;
    }

    private void setSsid(final String ssid) {
        this.mSsid = ssid;
    }

    public String getSsid() {
        return this.mSsid;
    }

    @Override
    public String toString() {
        return getSsid() + " " + getRssi();
    }

    @Override
    public boolean equals(Object o) {
        if (o instanceof SnifferNetworkInfo == false)
            return false;

        SnifferNetworkInfo other = (SnifferNetworkInfo) o;

        return Objects.equals(getSsid(), other.getSsid())
                && Objects.equals(getRssi(), other.getRssi());
    }

    /**
     * Provides a convenient way to set the fields of a {@link SnifferNetworkInfo}
     * when creating a new instance.
     *
     * <p>
     * The example below show how you might create a new {@code SnifferNetworkInfo}:
     *
     * <pre>
     * <code> 
     *
     * final int rssi = -87;
     * final String ssid = "Malbolge";
     *
     * SnifferNetworkInfo sni = new SnifferNetworkInfo.Builder()
     *          .setSsid(ssid)
     *          .setRssi(rssi)
     *          .build();
     *
     * </code>
     * </pre>
     */
    public static class Builder {
        private int rssi = 1;
        private String ssid;

        public Builder() {
        }

        public Builder setSsid(String ssid) {
            this.ssid = ssid;
            return this;
        }

        public Builder setRssi(int rssi) {
            this.rssi = rssi;
            return this;
        }

        /**
         * Builds {@link SnifferNetworkInfo} from this builder.
         *
         * @return {@code null} if {@link #setSsid(String)} is empty or null, or {@link #setRssi(int)} is
         *         greater than 0, {@code SnifferNetworkInfo} built from this builder
         *         otherwise.
         */
        public SnifferNetworkInfo Build() {
            Log.d(TAG, "Building SnifferNetworkInfo");

            if (TextUtils.isEmpty(this.ssid) || this.rssi > 0)
                return null;

            return new SnifferNetworkInfo(this);
        }

    }

}