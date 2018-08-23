package com.threestudio.ndk_knowledge_studio;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {
    private EditText threadsEdit;
    private EditText iterationsEdit;
    private Button startButton;
    private TextView logView;

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("threads_sample-lib");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        nativeInit();

        threadsEdit = (EditText)findViewById(R.id.threads_edit);
        iterationsEdit = (EditText)findViewById(R.id.iterations_edit);
        startButton = (Button)findViewById(R.id.start_button);
        logView = (TextView)findViewById(R.id.log_view);
        startButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                int threads = getNumber(threadsEdit,0);
                int iterations = getNumber(iterationsEdit,0);
                if(threads > 0 && iterations > 0)
                {
                    startThreads(threads,iterations);
                }
            }
        });

    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        nativeFree();

    }

    private void javaThread(int threads,final int iterations)
    {
        for(int i = 0;i < threads; i++)
        {
            final  int id = i;
            Thread thread = new Thread(){
                @Override
                public void run() {
                    nativeWorker(id,iterations);
                }
            };
            thread.start();

        }
    }



    private void onNativeMessage(final String message){
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                logView.append(message);
                logView.append("\n");
            }
        });
    }

    private void startThreads(int threads, int iterations) {
        //javaThread(threads,iterations);
        posixThreads(threads,iterations);

    }

    private int getNumber(EditText editText, int defaultValue) {
        int value;
        try{
            value = Integer.parseInt(editText.getText().toString());
        }catch(NumberFormatException e){
            value = defaultValue;
        }
        return value;
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native void nativeInit();
    public native void nativeFree();
    public native void nativeWorker(int id,int iterations);
    public native void posixThreads(int Threads,int iterations);
}
