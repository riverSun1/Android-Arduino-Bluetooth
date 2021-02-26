package com.example.controller;

// 원격 장치 정보에 대한 자리 표시 자 역할을하는 클래스입니다.
public class DeviceInfoModel {

    private String deviceName, deviceHardwareAddress;

    public DeviceInfoModel()
    {

    }

    public DeviceInfoModel(String deviceName, String deviceHardwareAddress){ // 장치 이름, MAC주소 표시
        this.deviceName = deviceName;
        this.deviceHardwareAddress = deviceHardwareAddress;
    }

    public String getDeviceName()
    {
        return deviceName;
    }

    public String getDeviceHardwareAddress()
    {
        return deviceHardwareAddress;
    }

}
