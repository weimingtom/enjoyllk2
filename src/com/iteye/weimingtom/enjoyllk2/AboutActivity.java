package com.iteye.weimingtom.enjoyllk2;

import android.app.Activity;
import android.content.ActivityNotFoundException;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.Toast;
import android.net.Uri;

public class AboutActivity extends Activity {
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		this.setContentView(R.layout.about);
		
		Button sendmail = (Button)this.findViewById(R.id.sendmail);
		sendmail.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				Intent intent = new Intent();
				intent.setAction(Intent.ACTION_SENDTO);
				intent.setData(Uri.parse("mailto:" + getString(R.string.mail_address)));
				intent.putExtra(Intent.EXTRA_SUBJECT, "关于 " + getString(R.string.app_name) + " v" + getString(R.string.version_number));
				//intent.putExtra(Intent.EXTRA_TEXT, "我想说：\n");
				try {
					startActivity(intent);
				} catch (ActivityNotFoundException e) {
					Toast.makeText(AboutActivity.this, "找不到发送邮件的应用程序。\n我的邮箱地址在帮助信息中。", Toast.LENGTH_SHORT).show();
					e.printStackTrace();
				}
			}
		});
		Button back = (Button)this.findViewById(R.id.back);
		back.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				finish();
			}
		});
	}
}
