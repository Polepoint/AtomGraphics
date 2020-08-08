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
import com.neo.atomgraphics.javascript.JSMessageCallback;
import com.neo.atomgraphics.javascript.JSMessageHandler;

import java.io.InputStream;
import java.io.InputStreamReader;
import java.nio.charset.StandardCharsets;

public class MultiGraphicsDemoScreen extends Fragment {

    private AtomGraphicsView mGraphicsView0;
    private AtomGraphicsView mGraphicsView1;
    private AtomGraphicsView mGraphicsView2;
    private JavaScriptExecutionEnvironment mEnvironment0;
    private JavaScriptExecutionEnvironment mEnvironment1;
    private JavaScriptExecutionEnvironment mEnvironment2;
    private boolean mChartLoaded;

    @Nullable
    @Override
    public View onCreateView(LayoutInflater inflater, @Nullable ViewGroup container, Bundle savedInstanceState) {
        return inflater.inflate(R.layout.fragment_multi_graphics, container, false);


    }

    @Override
    public void onViewCreated(View view, @Nullable Bundle savedInstanceState) {
        super.onViewCreated(view, savedInstanceState);
        mGraphicsView0 = this.getView().findViewById(R.id.multi_graphics_view_0);
        mGraphicsView1 = this.getView().findViewById(R.id.multi_graphics_view_1);
        mGraphicsView2 = this.getView().findViewById(R.id.multi_graphics_view_2);
        mEnvironment0 = new JavaScriptExecutionEnvironment();
        mEnvironment1 = new JavaScriptExecutionEnvironment();
        mEnvironment2 = new JavaScriptExecutionEnvironment();
        mGraphicsView0.setExecutionEnvironment(mEnvironment0);
        mGraphicsView1.setExecutionEnvironment(mEnvironment1);
        mGraphicsView2.setExecutionEnvironment(mEnvironment2);
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
                    FragmentTransaction transaction = MultiGraphicsDemoScreen.this.getFragmentManager().beginTransaction();
                    transaction.setCustomAnimations(R.anim.slide_in_left, R.anim.slide_out_right);
                    transaction.remove(MultiGraphicsDemoScreen.this);
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

                loadScript(mEnvironment0, "clock.js");
                loadScript(mEnvironment1, "flower.js");
                loadScript(mEnvironment2, "hanabi.js");

            }
        });

        return anim;
    }

    private void loadScript(JavaScriptExecutionEnvironment environment, String entryFileName) {
        String chartJS;
        String options;

        try {
            InputStream inputStream = getContext().getAssets().open(entryFileName);
            InputStreamReader streamReader = new InputStreamReader(inputStream, StandardCharsets.UTF_8);
            char input[] = new char[inputStream.available()];
            int bufferSize = streamReader.read(input);
            streamReader.close();
            chartJS = new String(input, 0, bufferSize);

        } catch (Exception e) {
            return;
        }

        environment.evaluateScript(chartJS);
    }

    @Override
    public void onDestroy() {
        super.onDestroy();

        mEnvironment0.callJSHandler("dispose");
        mEnvironment1.callJSHandler("dispose");
        mEnvironment2.callJSHandler("dispose");

        mGraphicsView0.destroy();
        mGraphicsView1.destroy();
        mGraphicsView2.destroy();
    }
}
