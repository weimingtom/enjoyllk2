package com.iteye.weimingtom.enjoyllk2;

import org.libsdl.app.SDLActivity;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;

public class MainMenuActivity extends Activity {
	private Button btnStart, btnAbout, btnExit;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		this.setContentView(R.layout.main_menu);
		
		btnStart = (Button)this.findViewById(R.id.btnStart);
		btnAbout = (Button)this.findViewById(R.id.btnAbout);
		btnExit = (Button)this.findViewById(R.id.btnExit);
		
		btnStart.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View view) {
				startActivity(new Intent(MainMenuActivity.this,
						SDLActivity.class));
			}
		});
		btnAbout.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View view) {
				about();
			}
		});		
		btnExit.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View view) {
				finish();
			}
		});
	}
	
	protected void about() {
		startActivity(new Intent(MainMenuActivity.this,
				AboutActivity.class));		
	}
}
