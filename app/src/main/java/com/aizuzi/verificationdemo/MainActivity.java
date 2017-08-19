package com.aizuzi.verificationdemo;

import android.content.Context;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import java.security.MessageDigest;
import java.util.Locale;

public class MainActivity extends AppCompatActivity {

  // Used to load the 'native-lib' library on application startup.
  static {
    System.loadLibrary("native-lib");
  }

  protected TextView appSignaturesTv;
  protected TextView jniSignaturesTv;
  protected Button checkBtn;
  protected Button tokenBtn;

  @Override
  protected void onCreate(Bundle savedInstanceState) {
    super.onCreate(savedInstanceState);
    super.setContentView(R.layout.activity_main);

    initView();

    appSignaturesTv.setText(getSha1Value(MainActivity.this));
    jniSignaturesTv.setText(getSignaturesSha1(MainActivity.this));
  }

  private View.OnClickListener clickListener = new View.OnClickListener(){
    @Override
    public void onClick(View v) {
      boolean result = checkSha1(MainActivity.this);

      if(result){
        Toast.makeText(getApplicationContext(),"验证通过",Toast.LENGTH_LONG).show();
      }else{
        Toast.makeText(getApplicationContext(),"验证不通过，请检查valid.cpp文件配置的sha1值",Toast.LENGTH_LONG).show();
      }
    }
  };

  private View.OnClickListener tokenClickListener = new View.OnClickListener(){
    @Override
    public void onClick(View v) {
      String result = getToken(MainActivity.this,"12345");

      Toast.makeText(getApplicationContext(),result,Toast.LENGTH_LONG).show();
    }
  };

  private void initView() {
    appSignaturesTv = (TextView) findViewById(R.id.app_signatures_tv);
    jniSignaturesTv = (TextView) findViewById(R.id.jni_signatures_tv);
    checkBtn = (Button) findViewById(R.id.check_btn);
    tokenBtn = (Button) findViewById(R.id.token_btn);

    checkBtn.setOnClickListener(clickListener);
    tokenBtn.setOnClickListener(tokenClickListener);
  }

  /**
   * A native method that is implemented by the 'native-lib' native library,
   * which is packaged with this application.
   */
  public native String getSignaturesSha1(Context context);
  public native boolean checkSha1(Context context);
  public native String getToken(Context context,String userId);


  public String getSha1Value(Context context) {
    try {
      PackageInfo info = context.getPackageManager().getPackageInfo(
          context.getPackageName(), PackageManager.GET_SIGNATURES);
      byte[] cert = info.signatures[0].toByteArray();
      MessageDigest md = MessageDigest.getInstance("SHA1");
      byte[] publicKey = md.digest(cert);
      StringBuffer hexString = new StringBuffer();
      for (int i = 0; i < publicKey.length; i++) {
        String appendString = Integer.toHexString(0xFF & publicKey[i])
            .toUpperCase(Locale.US);
        if (appendString.length() == 1)
          hexString.append("0");
        hexString.append(appendString);
      }
      String result = hexString.toString();
      return result.substring(0, result.length());
    } catch (Exception e) {
      e.printStackTrace();
    }
    return null;
  }
}
