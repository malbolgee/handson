package com.devtitans.sniffertest;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Context;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.util.Log;
import android.view.View;
import android.widget.Button;

import android.os.RemoteException;
import android.widget.TextView;

import com.devtitans.libs.net.SnifferNetworkInfo;
import com.devtitans.internal.SnifferManager;

public class MainActivity extends AppCompatActivity {

    private static final String TAG = "HandsOn";
    private static final int MESSAGE_UPDATE_TEXT_CHILD_THREAD = 1;

    private TextView mTextView;
    private Button mButton;
    private SnifferManager mSnifferManager;
    private Context mContext;
    private Handler mUpdateHandler;

    private String ssid;
    private int rssi;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        createUpdateUiHandler();

        mContext = getApplicationContext();

        mSnifferManager = SnifferManager.getInstance();

        mButton = findViewById(R.id.get_network_info_button);
        mTextView = findViewById(R.id.textView);

        mButton.setOnClickListener(this::onClick);
    }

    private void createUpdateUiHandler() {
        if (mUpdateHandler == null) {
            mUpdateHandler = new Handler(Looper.myLooper()) {
                @Override
                public void handleMessage(Message msg) {
                    if (msg.what == MESSAGE_UPDATE_TEXT_CHILD_THREAD) {
                        updateText(ssid, rssi);
                    }
                }
            };
        }
    }

    private void updateText(final String ssid, final int rssi) {
        final String text = mContext.getString(R.string.network_info, ssid, rssi);
        mTextView.setText(text);
        Log.d(TAG, "Finished updating view");
    }

    private void onClick(View v) {
        Thread workerThread = new Thread(() -> {

            final SnifferNetworkInfo netInfo;
            try {
                netInfo = mSnifferManager.getNetworkInfo();
            } catch (RemoteException re) {
                Log.d(TAG, "Caught remote exception on button click.");
                return;
            }

            if (netInfo == null) {
                Log.d(TAG, "Net info is null, doing nothing");
                return;
            }

            ssid = netInfo.getSsid();
            rssi = netInfo.getRssi();

            Message message = new Message();
            message.what = MESSAGE_UPDATE_TEXT_CHILD_THREAD;
            mUpdateHandler.sendMessage(message);
        });

        workerThread.start();
    }
}