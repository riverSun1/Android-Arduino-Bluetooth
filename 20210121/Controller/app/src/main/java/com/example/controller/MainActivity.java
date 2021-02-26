package com.example.controller;

import androidx.appcompat.app.AppCompatActivity;
import androidx.constraintlayout.widget.ConstraintLayout;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.text.Editable;
import android.text.TextWatcher;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.CompoundButton;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.Switch;
import android.widget.TextView;
import android.widget.Toast;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.UUID;

import static android.content.ContentValues.TAG;
import android.widget.Filter;

// ================================================================================================
public class MainActivity extends AppCompatActivity {
// ------------------------------------------------------------------------------------------------
    private String deviceName = null;
    private String deviceAddress;
    public static Handler handler;
    public static BluetoothSocket mmSocket;
    public static ConnectedThread connectedThread;
    public static CreateConnectThread createConnectThread;
    public String cmdText = "null";
    int count = 0;
    //String Edit;  // settingNum에 안드로이드 키패드로 입력할 숫자


    private final static int CONNECTING_STATUS = 1; // 메시지 상태를 식별하기 위해 블루투스 핸들러에서 사용
    private final static int MESSAGE_READ = 2; // 메시지 업데이트를 식별하기 위해 블루투스 핸들러에서 사용

// --------------------------------------------------------------------------------------------
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        final Button connectButton = (Button) findViewById(R.id.connectButton);
        final TextView textView = (TextView) findViewById(R.id.textView);
        final TextView now = (TextView) findViewById(R.id.now);
        final TextView setting = (TextView) findViewById(R.id.setting);
        final ImageView heatOn = (ImageView) findViewById(R.id.heatOn);
        final ImageView heatOff = (ImageView) findViewById(R.id.heatOff);
        final Switch sw = (Switch) findViewById(R.id.sw);
        Boolean switchState = sw.isChecked();
        final Button pbtn = (Button) findViewById(R.id.pbtn);
        final Button mbtn = (Button) findViewById(R.id.mbtn);

        final EditText nowNum = (EditText) findViewById(R.id.nowNum);
        nowNum.setText("0");
        final EditText settingNum = (EditText) findViewById(R.id.settingNum);
        settingNum.setText("0");

        deviceName = getIntent().getStringExtra("deviceName");
        if (deviceName != null) { // 데이터를 가져올 디바이스가 있다면
            // 블루투스 연결을 위한 장치 주소 가져 오기
            deviceAddress = getIntent().getStringExtra("deviceAddress");
            // "deviceAddress" : 전달할 데이터명(디바이스 주소)

            //textView.setText("Connecting to " + deviceName + "...");
            textView.setText("접속중");
            connectButton.setEnabled(false); // connect 버튼 비활성화

            /*
            이것은 가장 중요한 코드입니다. "deviceName"을 찾은 경우
            코드는 새 스레드를 호출하여 블루투스 연결을 생성합니다.
            선택한 장치 (아래 스레드 코드 참조)

            블루투스와 관련된 모든 작업은 BluetoothAdapter.getDefaultAdapter ( ) 함수를 이용하여
            BluetoothAdapter 클래스를 얻는 작업부터 시작합니다.
            만약 null을 반환하면 스마트폰에서 블루투스를 지원하지 않는 것입니다.
             */
            BluetoothAdapter bluetoothAdapter = BluetoothAdapter.getDefaultAdapter(); // BluetoothAdapter 객체 선언
            createConnectThread = new CreateConnectThread(bluetoothAdapter, deviceAddress);
            createConnectThread.start();

            pbtn.setOnClickListener(new Button.OnClickListener(){
                @Override
                public void onClick(View view)
                {
                    //int a = Integer.parseInt((settingNum.getText().toString()));
                    //int b = Integer.parseInt(a);
                    count++;
                    settingNum.setText(""+count);
                    //settingNum.setText(a+count);
                    //settingNum.setText(count);
                    cmdText = Integer.toString(count)+'\r'; // 정수를 문자열로 형변환
                    connectedThread.write(cmdText);
                }
            });

            mbtn.setOnClickListener(new Button.OnClickListener(){
                @Override
                public void onClick(View view)
                {
                    //int a = Integer.parseInt(String.valueOf(settingNum.getText()));
                    //int a = Integer.parseInt((settingNum.getText().toString()));
                    count--;
                    //settingNum.setText(count+ncount);
                    settingNum.setText(""+count);
                    //settingNum.setText(a+ncount);
                    cmdText = Integer.toString(count)+'\r'; // 정수를 문자열로 형변환
                    connectedThread.write(cmdText);
                }
            });

            sw.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
                @Override public void onCheckedChanged(CompoundButton buttonView, boolean isChecked){
                    if(isChecked)
                    {
                        heatOn.setVisibility(View.INVISIBLE);
                        heatOff.setVisibility(View.VISIBLE);
                    } else {
                        heatOn.setVisibility(View.VISIBLE);
                        heatOff.setVisibility(View.INVISIBLE);
                    }
                }
            });

        }

        handler = new Handler(Looper.getMainLooper()) {
            @Override
            public void handleMessage(Message msg) { // handler를 통해 실행결과를 Message로 전달
                switch (msg.what) { // message의 what값을 이용. 알맞게 UI처리
                    // what : 메시지를 수신하는 핸들러가 식별할 수 있는 사용자 정의 메시지 ID.
                    // 핸들러는 이 멤버 변수를 참조하여 어떠한 처리를 요청하는 메시지인지 구별할 수 있다.
                    case CONNECTING_STATUS: // 1, 연결된 상태일 때. 메세지 상태 식별
                        switch (msg.arg1) { // arg : 간단한 정수 값을 저장할 수 있는 멤버 변수
                            case 1: // 연결 성공
                                //textView.setText("Connected to " + deviceName);
                                textView.setText("접속되었음");
                                connectButton.setEnabled(true); // connect 버튼 활성화
                                break;
                            case -1: // 연결 실패
                                textView.setText("Device fails to connect");
                                connectButton.setEnabled(true); // connect 버튼 활성화
                                break;
                        }
                        break;

                    // https://freedeveloper.tistory.com/191 (제어문자 설명)
                    case MESSAGE_READ: // 2, 메세지 업데이트 식별
                        String arduinoMsg = msg.obj.toString(); // Read message from Arduino
                        //switch (arduinoMsg.toLowerCase()){ // toLowerCase(); : 문자열을 소문자로 변환해서 반환
                        switch (arduinoMsg.charAt(0)) {
                            case 'N':
                                //textView.setText("Blue LED Status : " + arduinoMsg);
                                nowNum.setText(arduinoMsg.toCharArray(), 1, 2);
                                break;
                            /*
                                case 'S':
                                //textView.setText("Blue LED Status : " + arduinoMsg);
                                //settingNum.setText("77");
                                settingNum.setText(arduinoMsg.toCharArray(), 1, 2);
                                break;

                             */
                            case 'H':
                                if (arduinoMsg.charAt(1) == '1')
                                {
                                    heatOn.setVisibility(View.INVISIBLE); // is 1
                                    heatOff.setVisibility(View.VISIBLE);
                                }
                                else {
                                    heatOn.setVisibility(View.VISIBLE);   // is 0
                                    heatOff.setVisibility(View.INVISIBLE);
                                }
                                break;
                            /*
                            case "N55\r":
                                //textView.setText("Blue LED Status : " + arduinoMsg);
                                nowNum.setText(arduinoMsg.toCharArray(), 1, 2);
                                break;
                            case "S77\r":
                                //textView.setText("Blue LED Status : " + arduinoMsg);
                                //settingNum.setText("77");
                                settingNum.setText(arduinoMsg.toCharArray(), 1, 2);
                                break;
                            case "H1\r":
                                heatOn.setVisibility(View.INVISIBLE);
                                heatOff.setVisibility(View.VISIBLE);
                                break;
                            case "H0\r":
                                heatOn.setVisibility(View.VISIBLE);
                                heatOff.setVisibility(View.INVISIBLE);
                                break;
                             */
                            default:
                                //textView.setText("Blue LED Status : empty &" + arduinoMsg);
                                break;
                        }
                        break;
                }
            }
        };

        // 블루투스 장치 선택
        connectButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                // Move to adapter list
                // connect 버튼을 누르면 블루투스 장치 선택하는 화면으로 넘어가라.
                Intent intent = new Intent(MainActivity.this, SelectDeviceActivity.class);
                startActivity(intent); // 인텐트에 대해 startActivity 수행
            }
        });
    }

    // --------------------------------------------------------------------------------------------
    // Thread to Create Bluetooth Connection
    // 블루투스 연결 생성 스레드
    public static class CreateConnectThread extends Thread {
        public CreateConnectThread(BluetoothAdapter bluetoothAdapter, String address) {
            // mmSocket이 최종이므로 나중에 mmSocket에 할당되는 임시 개체를 사용합니다.
            // 검색된 디바이스의 MAC address를 통한 BluetoothDevice 생성
            BluetoothDevice bluetoothDevice = bluetoothAdapter.getRemoteDevice(address);
            BluetoothSocket tmp = null;
            UUID uuid = bluetoothDevice.getUuids()[0].getUuid();

            try {
                /*
                주어진 BluetoothDevice에 연결할 BluetoothSocket을 가져옵니다.
                Android 장치의 종류로 인해 아래 방법은 다른 장치에서 작동하지 않을 수 있습니다.
                다른 방법을 사용해보십시오.
                 : tmp = device.createRfcommSocketToServiceRecord (MY_UUID);
                 */
                tmp = bluetoothDevice.createInsecureRfcommSocketToServiceRecord(uuid);
                // 위의 메소드를 호출하여 BluetoothServerSoket을 얻는다.
                // 연결은 원격 장비가 서버 소켓을 생성할 때 등록한 UUID와 매칭되는 UUID를 가지고
                // 연결 요청을 보낼 때만 연결을 수락한다.
                // 블루투스 통신은 소켓을 생성해서 통신을 한다. 소켓을 얻기 위해서 상단의 메소드를 활용한다.
                // bluetoothDevice의 가장 큰 역할은 원격장치와의 연결을 요청하고 소켓을 얻는 것이다.

            } catch (IOException e) {
                Log.e(TAG, "Socket's create() method failed", e);
            }
            mmSocket = tmp;
        }

        public void run() {
            // Cancel discovery because it otherwise slows down the connection.
            // 연결 속도가 느려지므로 검색을 취소하십시오.
            // cancelDiscovery() : 디바이스 연결 전에 디바이스 검색을 중지한다.
            // 안드로이드 dev 사이트에 보면 검색 작업은 리소스도 엄청 많이 먹고
            // 단말을 느리게 하므로 검색이 성공하거나 끝난 경우 cancelDiscovery()를 호출
            BluetoothAdapter bluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
            bluetoothAdapter.cancelDiscovery();
            try {
                // 소켓을 통해 원격 장치에 연결합니다.
                // 이 호출은 성공하거나 예외가 발생할 때까지 차단됩니다.
                mmSocket.connect(); // connect() : 원격 장치에 연결 시도
                Log.e("Status", "Device connected");
                handler.obtainMessage(CONNECTING_STATUS, 1, -1).sendToTarget();
            } catch (IOException connectException) {
                // 연결 실패시, 소켓을 닫고 리턴
                try {
                    mmSocket.close();
                    Log.e("Status", "Cannot connect to device");
                    handler.obtainMessage(CONNECTING_STATUS, -1, -1).sendToTarget();
                } catch (IOException closeException) {
                    Log.e(TAG, "Could not close the client socket", closeException);
                }
                return;
            }

            // 연결 시도가 성공했습니다. 별도의 스레드에서 연결과 관련된 작업을 수행합니다.
            connectedThread = new ConnectedThread(mmSocket);
            connectedThread.run();
        }

        // Closes the client socket and causes the thread to finish.
        // 클라이언트 소켓을 닫고 스레드를 완료합니다.
        public void cancel() {
            try {
                mmSocket.close();
            } catch (IOException e) {
                Log.e(TAG, "Could not close the client socket", e);
            }
        }
    }

    // --------------------------------------------------------------------------------------------
    // Thread for Data Transfer
    // 데이터 전송을위한 스레드
    public static class ConnectedThread extends Thread {
        private final BluetoothSocket mmSocket;
        private final InputStream mmInStream;
        private final OutputStream mmOutStream;

        // 블루투스 통신은 Socket을 이용해서 이루어진다. 소켓 통신은 InputStream, OutputStream 객체를 사용하여 데이터를 송수신한다.
        public ConnectedThread(BluetoothSocket socket) {
            mmSocket = socket;
            InputStream tmpIn = null;
            OutputStream tmpOut = null;
            // Get the input and output streams, using temp objects because member streams are final
            // 멤버 스트림이 최종이기 때문에 임시 객체를 사용하여 입력 및 출력 스트림을 가져옵니다.
            try {
                tmpIn = socket.getInputStream();
                tmpOut = socket.getOutputStream();
            } catch (IOException e) {
            }

            mmInStream = tmpIn;
            mmOutStream = tmpOut;
        }

        public void run() {
            // byte 단위 입출력 class 는 class의 이름이 InputStream, OutputStream으로 끝난다.
            // 파일로부터 1024byte씩 읽어들여 파일에 1024byte씩 저장
            // 1024만큼 읽어들이기위해 byte 배열을 사용
            byte[] buffer = new byte[1024];  // buffer store for the stream (스트림 용 버퍼 저장소) // 수신버퍼
            int bytes = 0; // bytes returned from read() (read ()에서 반환 된 바이트)
            // Keep listening to the InputStream until an exception occurs
            // 예외가 발생할 때까지 InputStream을 계속 청취하십시오.
            while (true) {
                try {
                    /*
                    종료 문자에 도달 할 때까지 Arduino의 InputStream에서 읽습니다.
                    그런 다음 전체 문자열 메시지를 GUI 처리기에 보냅니다.
                     */
                    buffer[bytes] = (byte) mmInStream.read();
                    String readMessage;
                    if (buffer[bytes] == '\n') {
                        readMessage = new String(buffer, 0, bytes);
                        Log.e("Arduino Message", readMessage);
                        handler.obtainMessage(MESSAGE_READ, readMessage).sendToTarget();
                        bytes = 0;
                    } else {
                        bytes++;
                    }
                } catch (IOException e) { // 데이터 수신 중 오류 발생
                    e.printStackTrace();
                    break;
                }
            }
        }

        /* Call this from the main activity to send data to the remote device */
        // main activity에서 이것을 호출하여 데이터를 원격 장치로 보냅니다.
        public void write(String input) { // 데이터를 보내는 것 // 파일에 쓰기
            byte[] bytes = input.getBytes(); //converts entered String into bytes (입력 된 문자열을 바이트로 변환)
            try {
                mmOutStream.write(bytes); // write() : 파일입출력? 모르겠음
            } catch (IOException e) {
                Log.e("Send Error", "Unable to send message", e);
            }
        }

        /* 연결을 종료하려면 기본 활동에서 이것을 호출하십시오. */
        public void cancel() {
            try {
                mmSocket.close();
            } catch (IOException e) {
            }
        }
    }

    // --------------------------------------------------------------------------------------------
    // Terminate Connection at BackPress
    @Override
    public void onBackPressed() // 뒤로가기 버튼을 눌렀을 경우 처리하는 함수
    {
        // Bluetooth 연결을 종료하고 앱을 닫습니다.
        if (createConnectThread != null){
            createConnectThread.cancel();
        }
        Intent a = new Intent(Intent.ACTION_MAIN);
        a.addCategory(Intent.CATEGORY_HOME); // 홈 화면으로 나가게하는 버튼(앱 아이콘이 보이는 화면)
        a.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
        // 새로운 태스크를 생성하여 그 태스크안에 액티비티 추가
        // Task - 어플리케이션에서 실행되는 액티비티를 보관하고 관리하며 Stack의 연속된 Activity로 이루어진다.
        // Flag - Flag를 사용하여 Task내 액티비티의 흐름을 제어할 수 있다.
        startActivity(a);
    }

}