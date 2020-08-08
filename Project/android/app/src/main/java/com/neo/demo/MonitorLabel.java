package com.neo.demo;

import android.content.Context;
import android.support.annotation.Nullable;
import android.util.AttributeSet;
import android.widget.LinearLayout;
import android.widget.TextView;

import com.neo.atomgraphics.VSyncMonitor;

public class MonitorLabel extends LinearLayout {

    private TextView m_textView;

    private VSyncMonitor mVSyncMonitor;

    private long mFrameCount = 0;

    private long mLastRecordedTime = 0;

    public MonitorLabel(Context context) {
        this(context, null);
    }

    public MonitorLabel(Context context, @Nullable AttributeSet attrs) {
        this(context, attrs, 0);
    }

    public MonitorLabel(Context context, @Nullable AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);

        m_textView = new TextView(context);
        LayoutParams layoutParams = new LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT);
        layoutParams.setMargins(14, 14, 14, 14);
        m_textView.setLayoutParams(layoutParams);
        m_textView.setBackgroundResource(R.drawable.round_corner);
        m_textView.setTextAlignment(TEXT_ALIGNMENT_CENTER);
        this.addView(m_textView);

        VSyncMonitor.Listener vSyncListener = new VSyncMonitor.Listener() {
            @Override
            public void onVSync(VSyncMonitor monitor, long vsyncTimeMicros) {
                onDisplayVSync(vsyncTimeMicros);
            }
        };

        this.mVSyncMonitor = new VSyncMonitor(context, vSyncListener);
    }

    @Override
    protected void onAttachedToWindow() {
        super.onAttachedToWindow();
        this.mVSyncMonitor.requestUpdate();
    }

    private void onDisplayVSync(long vsyncTimeMicros) {
        mFrameCount++;
        if (vsyncTimeMicros - mLastRecordedTime >= 984333) {
            updateLabelText(mFrameCount);
            mFrameCount = 0;
            mLastRecordedTime = vsyncTimeMicros;
        }

        this.mVSyncMonitor.requestUpdate();
    }

    private void updateLabelText(long fps) {
        CharSequence charSequence = "FPS: " + fps + "\nMem: " + Runtime.getRuntime().totalMemory() / (1024 * 1024) + "MB";
        m_textView.setText(charSequence);
    }
}
