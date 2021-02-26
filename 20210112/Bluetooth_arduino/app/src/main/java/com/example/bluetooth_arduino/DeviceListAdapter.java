package com.example.bluetooth_arduino;

import android.content.Context;
import android.content.Intent;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.LinearLayout;
import android.widget.TextView;

import androidx.recyclerview.widget.RecyclerView;

import java.util.List;

// Activity_select_devise.xml
// 연결할 수 있도록 페어링 된 Bluetooth 장치 목록을 표시하는 클래스입니다. 목록이 SelectDeviceActivity에 표시됩니다.
public class DeviceListAdapter extends RecyclerView.Adapter<RecyclerView.ViewHolder> {

    private Context context;
    // 어떤 객체를 핸들링하기 위한 접근 수단, 어떤 Activity 또는 어떤 application인가에 대해 구별하는 정보
    private List<Object> deviceList;
    // 제네릭은 클래스 내부에서 사용할 데이터 타입을 외부에서 지정하는 기법을 의미한다.
    // 클래스를 정의 할 때는 info의 데이터 타입을 확정하지 않고 인스턴스를 생성할 때 데이터 타입을 지정하는 기능이 제네릭이다
    // 그냥 <> 안에 제한하고 싶은 타입을 써


    // RecyclerView : 많은 수의 데이터 집합을, 제한된 영역 내에서 유연하게(flexible) 표시할 수 있도록 만들어주는 위젯
    // 리사이클러뷰(RecyclerView)는 "사용자가 관리하는 많은 수의 데이터 집합(Data Set)을
    // 개별 아이템 단위로 구성하여 화면에 출력하는 뷰그룹(ViewGroup)이며,
    // 한 화면에 표시되기 힘든 많은 수의 데이터를 스크롤 가능한 리스트로 표시해주는 위젯"입니다.
    public static class ViewHolder extends RecyclerView.ViewHolder {
        TextView textName, textAddress;
        LinearLayout linearLayout;

        public ViewHolder(View v) {
            super(v);
            textName = v.findViewById(R.id.textViewDeviceName);
            textAddress = v.findViewById(R.id.textViewDeviceAddress);
            linearLayout = v.findViewById(R.id.linearLayoutDeviceInfo);
        }
    }

    public DeviceListAdapter(Context context, List<Object> deviceList) {
        this.context = context;
        this.deviceList = deviceList;

    }

    public RecyclerView.ViewHolder onCreateViewHolder(ViewGroup parent, int viewType) {
        View v = LayoutInflater.from(parent.getContext()).inflate(R.layout.device_info_layout, parent, false);
        ViewHolder vh = new ViewHolder(v);
        return vh;
    }

    @Override
    public void onBindViewHolder(final RecyclerView.ViewHolder holder, final int position) {
        ViewHolder itemHolder = (ViewHolder) holder;
        final DeviceInfoModel deviceInfoModel = (DeviceInfoModel) deviceList.get(position);
        itemHolder.textName.setText(deviceInfoModel.getDeviceName());
        itemHolder.textAddress.setText(deviceInfoModel.getDeviceHardwareAddress());

        // When a device is selected
        // 장치가 선택되었을 때
        itemHolder.linearLayout.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent intent = new Intent(context,MainActivity.class);
                // Send device details to the MainActivity
                intent.putExtra("deviceName", deviceInfoModel.getDeviceName());
                intent.putExtra("deviceAddress",deviceInfoModel.getDeviceHardwareAddress());
                // Call MainActivity
                context.startActivity(intent);

            }
        });
    }

    @Override
    public int getItemCount() {
        int dataCount = deviceList.size();
        return dataCount;
    }
}
