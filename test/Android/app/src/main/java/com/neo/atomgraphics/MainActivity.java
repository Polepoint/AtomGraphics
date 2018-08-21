package com.neo.atomgraphics;

import android.graphics.Color;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.view.ViewGroup;
import android.view.Window;
import android.widget.Button;
import android.widget.LinearLayout;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("atomgraphics");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        setContentView(R.layout.activity_main);
        ViewGroup mainLayout = this.findViewById(R.id.top_layout);

        //toolbar
        LinearLayout topToolbar = new LinearLayout(this);
        topToolbar.setBackgroundColor(Color.rgb(55, 55, 55));
        topToolbar.setGravity(LinearLayout.HORIZONTAL);
        LinearLayout.LayoutParams toolbarLayoutParams = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, 110);
        topToolbar.setLayoutParams(toolbarLayoutParams);
        mainLayout.addView(topToolbar);

        //reload button
        Button reloadButton = new Button(this);
        LinearLayout.LayoutParams reloadButtonParams = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.MATCH_PARENT);
        reloadButtonParams.weight = 1;
        reloadButton.setLayoutParams(reloadButtonParams);
        CharSequence buttonText = "Reload";
        reloadButton.setText(buttonText);
        topToolbar.addView(reloadButton);

        //FPS
        FPSLabel fpsLabel = new FPSLabel(this);
        LinearLayout.LayoutParams fpsLabelLayoutParams = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.MATCH_PARENT);
        fpsLabelLayoutParams.weight = 1;
        fpsLabel.setLayoutParams(fpsLabelLayoutParams);
        topToolbar.addView(fpsLabel);

        //GraphicsView
        final AtomGraphicsView graphicsView = new AtomGraphicsView(this);
        LinearLayout.LayoutParams layoutParams = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.MATCH_PARENT);
        graphicsView.setLayoutParams(layoutParams);
        mainLayout.addView(graphicsView);

        reloadButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                graphicsView.reload();
            }
        });
    }
}
