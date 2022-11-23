package com.devtitans.sniffernetworkinfo;

import android.os.Bundle;
import android.os.Handler;
import android.os.RemoteException;
import android.util.Log;
import android.util.Pair;

import androidx.appcompat.app.AppCompatActivity;
import androidx.recyclerview.widget.LinearLayoutManager;

import com.devtitans.internal.SnifferManager;
import com.devtitans.libs.net.SnifferNetworkInfo;
import com.devtitans.sniffernetworkinfo.databinding.ActivityMainBinding;

import java.util.ArrayList;
import java.util.Collection;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Objects;
import java.util.Set;
import java.util.Timer;
import java.util.TimerTask;
import java.util.TreeSet;

public class MainActivity extends AppCompatActivity {

    private static final String TAG = MainActivity.class.getSimpleName();

    private HashMap<String, List<Integer>> mNetworkNamesAndSignalValues;
    private Set<Pair<String, Integer>> mNetworkNamesAndSignalMeans;
    private SnifferManager mSnifferManager;
    private ActivityMainBinding mViewBinding;
    private NetworkAdapter mNetworkAdapter;

    private List<SnifferNetworkInfo> networks;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        mViewBinding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(mViewBinding.getRoot());
        mSnifferManager = SnifferManager.getInstance();
        mNetworkAdapter = new NetworkAdapter(new ArrayList<>());
        mNetworkNamesAndSignalValues = new HashMap<>();
        networks = new ArrayList<>();

        setList(new HashSet<>());
        initRecyclerView();

        Timer networkListTimer = new Timer ();
        TimerTask networkListTask = new TimerTask () {
            public void run() {
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        updateList();
                    }
                });
            }
        };
        networkListTimer.scheduleAtFixedRate(networkListTask , 0l,  (10*1000));

        Timer updateNetworkTimer = new Timer ();
        TimerTask updateNetworkTask = new TimerTask () {
            public void run() {
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        getNetworks();
                    }
                });
            }
        };
        updateNetworkTimer.scheduleAtFixedRate(updateNetworkTask , 0l,  (2*1000));
    }

    private void initRecyclerView() {
        mViewBinding.recyclerView.setLayoutManager(new LinearLayoutManager(this));
        mViewBinding.recyclerView.setHasFixedSize(true);
        mNetworkAdapter.setNetworkNamesAndSignal(new ArrayList<>(getList()));
        mViewBinding.recyclerView.setAdapter(mNetworkAdapter);
    }

    private void setList(Set<Pair<String, Integer>> l) {
        mNetworkNamesAndSignalMeans = l;
    }

    private Set<Pair<String, Integer>> getList() {
        return this.mNetworkNamesAndSignalMeans;
    }

    private void getNetworks() {
        SnifferNetworkInfo network = null;
        try {
            network = mSnifferManager.getNetworkInfo();
        } catch (RemoteException e) {
            e.printStackTrace();
        }
        networks.add(network);
        keepValues(networks);
    }

    public void updateList() {
        var ss = getNetworkSignalAverage();
        setList(ss);
        mNetworkAdapter.setNetworkNamesAndSignal(new ArrayList<>(getList()));
        mNetworkAdapter.notifyDataSetChanged();
    }

    private Set<Pair<String, Integer>> getNetworkSignalAverage() {
        Comparator<Pair<String, Integer>> c = (o1, o2) -> {
            if (Objects.equals(o1.second, o2.second))
                return o1.first.compareToIgnoreCase(o2.first);
            else
                return o2.second - o1.second;
        };

        TreeSet<Pair<String, Integer>> s = new TreeSet<>(c);

        for (var item : mNetworkNamesAndSignalValues.entrySet()) {
            var mean = item.getValue()
                    .stream()
                    .mapToDouble(d -> d)
                    .average()
                    .orElse(0);

            s.add(new Pair<>(item.getKey(), (int)mean));
        }
        return s;
    }

    private void keepValues(List<SnifferNetworkInfo> s) {
        s.stream()
                .filter(i -> !(i.getRssi() == 0))
                .forEach(item -> {
                    final String key = item.getSsid();
                    final int value = item.getRssi();
                    if (mNetworkNamesAndSignalValues.containsKey(key))
                        mNetworkNamesAndSignalValues.get(key).add(value);
                    else
                        mNetworkNamesAndSignalValues.put(key, new ArrayList<>(List.of(value)));
                });

    }
}