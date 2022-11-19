package com.devtitans.sniffernetworkinfo;

import android.util.Pair;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

import androidx.annotation.NonNull;
import androidx.recyclerview.widget.RecyclerView;

import java.util.List;

public class NetworkAdapter extends RecyclerView.Adapter<NetworkAdapter.NetworkViewHolder> {

    private List<Pair<String, Integer>> mNetworkNamesAndSignal;

    public NetworkAdapter(List<Pair<String, Integer>> networkNamesAndSignal) {
        this.mNetworkNamesAndSignal = networkNamesAndSignal;
    }

    public void setNetworkNamesAndSignal(List<Pair<String, Integer>> list) {
        this.mNetworkNamesAndSignal = list;
    }

    public List<Pair<String, Integer>> getNetworkNamesAndSignal() {
        return this.mNetworkNamesAndSignal;
    }

    @NonNull
    @Override
    public NetworkViewHolder onCreateViewHolder(@NonNull ViewGroup parent, int viewType) {
        var itemView = LayoutInflater
                .from(parent.getContext())
                .inflate(R.layout.network_adapter, parent, false);

        return new NetworkViewHolder(itemView);
    }

    @Override
    public void onBindViewHolder(@NonNull NetworkViewHolder holder, int position) {
        var networkInfo = mNetworkNamesAndSignal.get(position);

        final int networkSignal = networkInfo.second;
        final int resourceId;

        if (networkSignal > -67) {
            resourceId = R.drawable.ic_wifi_100_signal;
        } else if (networkSignal > -70) {
            resourceId = R.drawable.ic_wifi_80_signal;
        } else if (networkSignal > -80) {
            resourceId = R.drawable.ic_wifi_20_signal;
        } else {
            resourceId = R.drawable.ic_wifi_10_signal;
        }

        holder.mNetworkSignal.setImageResource(resourceId);
        holder.mNetworkName.setText(networkInfo.first);
        holder.mNetworkValue.setText(Integer.toString(networkSignal));
    }

    @Override
    public int getItemCount() {
        return this.mNetworkNamesAndSignal.size();
    }

    public static class NetworkViewHolder extends RecyclerView.ViewHolder {

        private TextView mNetworkName;
        private ImageView mNetworkSignal;
        private TextView mNetworkValue;


        public NetworkViewHolder(@NonNull View itemView) {
            super(itemView);

            mNetworkName = itemView.findViewById(R.id.network_ssid);
            mNetworkSignal = itemView.findViewById(R.id.network_rssi);
            mNetworkValue = itemView.findViewById((R.id.rssid_value));
        }
    }

}
