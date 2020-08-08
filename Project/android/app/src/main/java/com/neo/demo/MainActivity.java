package com.neo.demo;

import android.support.v4.app.Fragment;
import android.graphics.Color;
import android.graphics.drawable.ColorDrawable;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.*;

import com.neo.atomgraphics.GraphicsEnvironment;
import com.neo.atomgraphics.GraphicsJavaScriptCore;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class MainActivity extends AppCompatActivity {
    private PopupWindow mWindow;

    private static List<Map<String, Object>> sDemoList;

    private static final String kDemoKeyName = "name";
    private static final String kDemoKeyClass = "class";

    static {
        sDemoList = new ArrayList<Map<String, Object>>();
        HashMap<String, Object> demoConfig;

        demoConfig = new HashMap<String, Object>();
        demoConfig.put(kDemoKeyName, "Tadpoles");
        demoConfig.put(kDemoKeyClass, TadpolesDemoScreen.class);
        sDemoList.add(demoConfig);

        demoConfig = new HashMap<String, Object>();
        demoConfig.put(kDemoKeyName, "Loading");
        demoConfig.put(kDemoKeyClass, LoadingDemoScreen.class);
        sDemoList.add(demoConfig);

        demoConfig = new HashMap<String, Object>();
        demoConfig.put(kDemoKeyName, "Loading2");
        demoConfig.put(kDemoKeyClass, Loading2DemoScreen.class);
        sDemoList.add(demoConfig);

        demoConfig = new HashMap<String, Object>();
        demoConfig.put(kDemoKeyName, "Flower");
        demoConfig.put(kDemoKeyClass, FlowerDemoScreen.class);
        sDemoList.add(demoConfig);

        demoConfig = new HashMap<String, Object>();
        demoConfig.put(kDemoKeyName, "Heart");
        demoConfig.put(kDemoKeyClass, HeartDemoScreen.class);
        sDemoList.add(demoConfig);

        demoConfig = new HashMap<String, Object>();
        demoConfig.put(kDemoKeyName, "Hanabi");
        demoConfig.put(kDemoKeyClass, HanabiDemoScreen.class);
        sDemoList.add(demoConfig);

        demoConfig = new HashMap<String, Object>();
        demoConfig.put(kDemoKeyName, "Colorful Text");
        demoConfig.put(kDemoKeyClass, ColorfulTextDemoScreen.class);
        sDemoList.add(demoConfig);

        demoConfig = new HashMap<String, Object>();
        demoConfig.put(kDemoKeyName, "Clock");
        demoConfig.put(kDemoKeyClass, ClockDemoScreen.class);
        sDemoList.add(demoConfig);

        demoConfig = new HashMap<String, Object>();
        demoConfig.put(kDemoKeyName, "Pixels Graphic");
        demoConfig.put(kDemoKeyClass, PixelsGraphicDemoScreen.class);
        sDemoList.add(demoConfig);


        demoConfig = new HashMap<String, Object>();
        demoConfig.put(kDemoKeyName, "Galaxy");
        demoConfig.put(kDemoKeyClass, GalaxyDemoScreen.class);
        sDemoList.add(demoConfig);

        demoConfig = new HashMap<String, Object>();
        demoConfig.put(kDemoKeyName, "Multiple Graphics");
        demoConfig.put(kDemoKeyClass, MultiGraphicsDemoScreen.class);
        sDemoList.add(demoConfig);
    }

    private static String getDemoName(int index) {
        return (String) sDemoList.get(index).get(kDemoKeyName);
    }

    private static Fragment getDemoFragmentInstance(int index) {
        Fragment fragment = null;

        try {
            fragment = ((Class<Fragment>) sDemoList.get(index).get(kDemoKeyClass)).newInstance();
        } catch (Exception e) {

        }

        return fragment;
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        GraphicsEnvironment.initialize(getApplicationContext());


        setContentView(R.layout.activity_main);
        ListView listView = findViewById(R.id.demo_list);
        BaseAdapter adapter = new BaseAdapter() {
            @Override
            public int getCount() {
                return sDemoList.size();
            }

            @Override
            public Object getItem(int position) {
                return null;
            }

            @Override
            public long getItemId(int position) {
                return 0;
            }

            @Override
            public View getView(int position, View convertView, ViewGroup parent) {
                View cellView;
                if (convertView == null) {
                    LayoutInflater inflater = MainActivity.this.getLayoutInflater();
                    cellView = inflater.inflate(R.layout.list_item, null);
                } else {
                    cellView = convertView;
                }

                TextView cellTitle = cellView.findViewById(R.id.cell_title);
                cellTitle.setText(getDemoName(position));

                return cellView;
            }
        };

        listView.setAdapter(adapter);
        listView.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
                getSupportFragmentManager()
                        .beginTransaction()
                        .setCustomAnimations(R.anim.slide_in_right, R.anim.slide_out_left)
                        .add(R.id.fl_ct, getDemoFragmentInstance(position))
                        .commit();
            }
        });
    }

    @Override
    public void onAttachedToWindow() {
        super.onAttachedToWindow();
        if (mWindow == null) {
            MonitorLabel monitorLabel = new MonitorLabel(this);
            monitorLabel.setLayoutParams(new ViewGroup.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT, ViewGroup.LayoutParams.WRAP_CONTENT));
            mWindow = new PopupWindow(monitorLabel);
            mWindow.setWindowLayoutMode(ViewGroup.LayoutParams.WRAP_CONTENT, ViewGroup.LayoutParams.WRAP_CONTENT);
            mWindow.setBackgroundDrawable(new ColorDrawable(Color.TRANSPARENT));
            mWindow.setOutsideTouchable(false);
            mWindow.setTouchable(false);
            View view = this.findViewById(R.id.fl_ct);
            mWindow.showAsDropDown(view, 0, getResources().getDimensionPixelSize(R.dimen.monitor_window_y_offset));
        }
    }

}
