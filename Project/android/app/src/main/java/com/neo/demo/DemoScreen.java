package com.neo.demo;


import android.support.v4.app.Fragment;
import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.v4.app.FragmentTransaction;
import android.view.KeyEvent;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.animation.Animation;
import android.view.animation.AnimationUtils;

import com.neo.atomgraphics.AtomGraphicsView;
import com.neo.atomgraphics.execution.JavaScriptExecutionEnvironment;

import java.io.InputStream;
import java.io.InputStreamReader;
import java.nio.charset.StandardCharsets;

public class DemoScreen extends Fragment {

    private AtomGraphicsView mGraphicsView;
    private JavaScriptExecutionEnvironment mEnvironment;
    private boolean mChartLoaded;

    @Nullable
    @Override
    public View onCreateView(LayoutInflater inflater, @Nullable ViewGroup container, Bundle savedInstanceState) {
        return inflater.inflate(R.layout.fragment_common_demo, container, false);
    }

    @Override
    public void onViewCreated(View view, @Nullable Bundle savedInstanceState) {
        super.onViewCreated(view, savedInstanceState);
        mGraphicsView = this.getView().findViewById(R.id.atgview);
        mEnvironment = new JavaScriptExecutionEnvironment();
        mGraphicsView.setExecutionEnvironment(mEnvironment);
    }

    @Override
    public void onResume() {
        super.onResume();
        getView().setFocusableInTouchMode(true);
        getView().requestFocus();
        getView().setOnKeyListener(new View.OnKeyListener() {
            @Override
            public boolean onKey(View view, int i, KeyEvent keyEvent) {
                if (keyEvent.getAction() == KeyEvent.ACTION_DOWN && i == KeyEvent.KEYCODE_BACK) {
                    FragmentTransaction transaction = DemoScreen.this.getFragmentManager().beginTransaction();
                    transaction.setCustomAnimations(R.anim.slide_in_left, R.anim.slide_out_right);
                    transaction.remove(DemoScreen.this);
                    transaction.commit();
                    return true;
                }
                return false;
            }
        });

    }

    @Override
    public Animation onCreateAnimation(int transit, boolean enter, int nextAnim) {

        Animation anim = AnimationUtils.loadAnimation(getActivity(), nextAnim);

        anim.setAnimationListener(new Animation.AnimationListener() {

            public void onAnimationStart(Animation animation) {

            }

            public void onAnimationRepeat(Animation animation) {

            }

            public void onAnimationEnd(Animation animation) {
                if (mChartLoaded) {
                    return;
                }
                mChartLoaded = true;

                loadScript(mGraphicsView);
            }
        });

        return anim;
    }

    private void loadScript(AtomGraphicsView graphicsView) {
        String chartJS;

        try {
            InputStream inputStream = getContext().getAssets().open(scriptFileName());
            InputStreamReader streamReader = new InputStreamReader(inputStream, StandardCharsets.UTF_8);
            char input[] = new char[inputStream.available()];
            int bufferSize = streamReader.read(input);
            streamReader.close();
            chartJS = new String(input, 0, bufferSize);
        } catch (Exception e) {
            return;
        }

        mEnvironment.evaluateScript(chartJS);
    }

    protected String scriptFileName() {
        return "";
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
        mEnvironment.callJSHandler("dispose");
        mGraphicsView.destroy();
    }
}
