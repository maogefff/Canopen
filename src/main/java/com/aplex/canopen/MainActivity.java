package com.aplex.canopen;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

public class MainActivity extends Activity {

    private static final String TAG = "MainActivity";
    Button bt;
    TextView DeviceType;
    TextView ErrorRegister;
    TextView ManufacturerStatusRegister;
    TextView PredefinedrerrorField;
    TextView COB_IDSyncMessage;
    TextView CommunicationCyclePeriod;
    TextView SynchronousWindowsLlength;
    TextView ManufacturerDeviceName;


    // Used to load the 'native-lib' library on application startup.
//    static {
//        System.loadLibrary("Canopencommand");
//    }
    CANopen caNopen = new CANopen();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        Log.d(TAG, "onCreate");
        setContentView(R.layout.activity_main);

        // Example of a call to a native method
        TextView tv = (TextView) findViewById(R.id.sample_text);
        //tv.setText(caNopen.stringFromJNI());

        bt = (Button) findViewById(R.id.bt);

        DeviceType = (TextView)findViewById(R.id.DeviceType);
        ErrorRegister = (TextView)findViewById(R.id.ErrorRegister);
        ManufacturerStatusRegister = (TextView)findViewById(R.id.ManufacturerStatusRegister);
        PredefinedrerrorField = (TextView)findViewById(R.id.PredefinedrerrorField);
        COB_IDSyncMessage = (TextView)findViewById(R.id.COB_IDSyncMessage);
        CommunicationCyclePeriod = (TextView)findViewById(R.id.CommunicationCyclePeriod);
        SynchronousWindowsLlength = (TextView)findViewById(R.id.SynchronousWindowsLlength);
        ManufacturerDeviceName = (TextView)findViewById(R.id.ManufacturerDeviceName);

        bt.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                //caNopen.excuteUtils()
            }
        });
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
//    public native String stringFromJNI();
//    public native String excuteUtils(int argc, String[] argv);
}
