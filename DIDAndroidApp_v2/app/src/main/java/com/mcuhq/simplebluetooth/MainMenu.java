package com.mcuhq.simplebluetooth;

import android.content.Intent;
import android.support.design.widget.CoordinatorLayout;
import android.support.design.widget.Snackbar;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.LinearLayout;

public class MainMenu extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main_menu);

        Button button = (Button) this.findViewById(R.id.game);
        button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                    Intent i = new Intent(getApplicationContext(), MainActivity.class);
                    startActivity(i);
            }
        });

        Button button2 = (Button) this.findViewById(R.id.control);
        button2.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent i = new Intent(getApplicationContext(), MainActivity.class); // need to change to ManualControl.class
                startActivity(i);
            }
        });

        Button button3 = (Button) this.findViewById(R.id.guard);
        button3.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent i = new Intent(getApplicationContext(), MainActivity.class);  // need to change to GuardMode.class
                startActivity(i);
            }
        });

        Button button4 = (Button) this.findViewById(R.id.setting);
        button4.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent i = new Intent(getApplicationContext(), Setting.class);
                startActivity(i);
            }
        });

    }

    /* this is put in the home main menu  to allow for press double to end app */
    boolean doubleBackPressed=false;
    @Override
    public void onBackPressed() {
        if(doubleBackPressed) {
            super.onBackPressed();
        }
        else {
            doubleBackPressed=true;
            final LinearLayout topLayout = (LinearLayout) findViewById(R.id.topLayout);

            Snackbar.make(topLayout, getString(R.string.pressbackagain), Snackbar.LENGTH_SHORT).show();
            new android.os.Handler().postDelayed(new Runnable() {
                @Override
                public void run() {
                    doubleBackPressed = false;
                }
            },2000);
        }
    }

}
