package com.rfstar.kevin.params;

import java.io.Serializable;

import com.rfstar.kevin.app.App;
import com.rfstar.kevin.tools.PreferenceHelper;
import com.rfstar.kevin.tools.Tools;

import android.annotation.TargetApi;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothGattCharacteristic;
import android.bluetooth.BluetoothGattService;
import android.content.Context;

import android.os.Build;
import android.util.Log;

@TargetApi(Build.VERSION_CODES.JELLY_BEAN_MR2)
public class CubicBLEDevice extends BLEDevice implements Serializable {

	private  Context context;

	public CubicBLEDevice(Context context, BluetoothDevice bluetoothDevice) {
		// TODO Auto-generated constructor stub
		super(context, bluetoothDevice);
		this.context =context;
	}

	@Override
	protected void deviceConnect() {

	}

	@Override
	protected void discoverCharacteristicsFromService() {
		// TODO Auto-generated method stub
		Log.d(App.TAG, "load all the services ");

		Log.e("wang3", "mac:" + device.getAddress());
		for (BluetoothGattService bluetoothGattService : bleService
				.getSupportedGattServices(device)) {
			String serviceUUID = Long.toHexString(
					bluetoothGattService.getUuid().getMostSignificantBits())
					.substring(0, 4);
			for (BluetoothGattCharacteristic bluetoothGattCharacteristic : bluetoothGattService
					.getCharacteristics()) {
				String characterUUID = Long.toHexString(
						bluetoothGattCharacteristic.getUuid()
								.getMostSignificantBits()).substring(0, 4);
				if ("ffc0".equals(serviceUUID)
						&& "ffc1".equals(characterUUID)) {
					String password = PreferenceHelper.get(context,device.getAddress());
					if((password != null)&&(password.length()>=6)){
						bluetoothGattCharacteristic.setValue(stringToByte(password+password));
						try {
							Thread.sleep(20);
							bluetoothGattCharacteristic
									.setWriteType(bluetoothGattCharacteristic.WRITE_TYPE_NO_RESPONSE);
							this.writeValue(bluetoothGattCharacteristic);
							Log.e("wang3", "mac:" + device.getAddress() + "password:" + password);
						} catch (InterruptedException e) {
							e.printStackTrace();
						}

					}

				}
			}
		}


		// BluetoothGattService gattService = new BluetoothGattService(uuid,
		// serviceType);
		// UUID uuid = UUID.fromString("0000ffe9-0000-1000-8000-00805f9b34fb");
		// BluetoothGattCharacteristic characteristic = new
		// BluetoothGattCharacteristic(
		// uuid, properties, permissions);
	}

	/**
	 * 
	 * @param serviceUUID
	 * @param characteristicUUID
	 * @param data
	 */
	public void writeValue(String serviceUUID, String characteristicUUID,
			byte[] value) {
		// TODO Auto-generated method stub
		for (BluetoothGattService bluetoothGattService : bleService
				.getSupportedGattServices(device)) {
			String gattServiceUUID = Long.toHexString(
					bluetoothGattService.getUuid().getMostSignificantBits())
					.substring(0, 4);
			for (BluetoothGattCharacteristic bluetoothGattCharacteristic : bluetoothGattService
					.getCharacteristics()) {
				String characterUUID = Long.toHexString(
						bluetoothGattCharacteristic.getUuid()
								.getMostSignificantBits()).substring(0, 4);
				if (gattServiceUUID.equals(serviceUUID)
						&& characteristicUUID.equals(characterUUID)) {
					// bluetoothGattCharacteristic.setValue(value);
					// this.writeValue(bluetoothGattCharacteristic);
					int length = value.length;
					int lengthChar = 0;
					int position = 0;
					while (length > 0) {
						if (length > 20) {
							lengthChar = 20;
						} else if (length > 0) {
							lengthChar = length;
						} else {
							return;
						}
						byte sendValue[] = new byte[lengthChar];
						for (int count = 0; count < lengthChar; count++) {
							sendValue[count] = value[position + count];
						}
						Log.e("wang3", "send:  " + Tools.byte2Hex(sendValue));
						// = value.substring(position, lengthChar+ position);
						bluetoothGattCharacteristic.setValue(sendValue);
						try {
							Thread.sleep(20);
							bluetoothGattCharacteristic
									.setWriteType(bluetoothGattCharacteristic.WRITE_TYPE_NO_RESPONSE);
							this.writeValue(bluetoothGattCharacteristic);
							length -= lengthChar;
							position += lengthChar;
						} catch (InterruptedException e) {
							e.printStackTrace();
						}

					}
				}
			}
		}
	}

	/**
	 * 
	 * @param serviceUUID
	 * @param characteristicUUID
	 * @param data
	 */
	public void writeValue(String serviceUUID, String characteristicUUID,
			String value) {
		// TODO Auto-generated method stub
		for (BluetoothGattService bluetoothGattService : bleService
				.getSupportedGattServices(device)) {
			String gattServiceUUID = Long.toHexString(
					bluetoothGattService.getUuid().getMostSignificantBits())
					.substring(0, 4);
			for (BluetoothGattCharacteristic bluetoothGattCharacteristic : bluetoothGattService
					.getCharacteristics()) {
				String characterUUID = Long.toHexString(
						bluetoothGattCharacteristic.getUuid()
								.getMostSignificantBits()).substring(0, 4);
				if (gattServiceUUID.equals(serviceUUID)
						&& characteristicUUID.equals(characterUUID)) {

					int length = value.length();
					int lengthChar = 0;
					int position = 0;
					while (length > 0) {
						if (length > 20) {
							lengthChar = 20;
						} else if (length > 0) {
							lengthChar = length;
						} else {
							return;
						}
						String sendValue = value.substring(position, lengthChar
								+ position);
						bluetoothGattCharacteristic.setValue(sendValue);
						try {
							Thread.sleep(20);
						} catch (InterruptedException e) {
							e.printStackTrace();
						}
						bluetoothGattCharacteristic
								.setWriteType(bluetoothGattCharacteristic.WRITE_TYPE_NO_RESPONSE);
						this.writeValue(bluetoothGattCharacteristic);

						length -= lengthChar;
						position += lengthChar;
					}
				}
			}
		}
	}

	/**
	 * 
	 * @param serviceUUID
	 * @param characteristicUUID
	 */
	public void readValue(String serviceUUID, String characteristicUUID) {
		for (BluetoothGattService bluetoothGattService : bleService
				.getSupportedGattServices(device)) {
			String gattServiceUUID = Long.toHexString(
					bluetoothGattService.getUuid().getMostSignificantBits())
					.substring(0, 4);
			for (BluetoothGattCharacteristic bluetoothGattCharacteristic : bluetoothGattService
					.getCharacteristics()) {
				String characterUUID = Long.toHexString(
						bluetoothGattCharacteristic.getUuid()
								.getMostSignificantBits()).substring(0, 4);
				if (gattServiceUUID.equals(serviceUUID)
						&& characteristicUUID.equals(characterUUID)) {
					Log.d(App.TAG, "charaterUUID read is success  : "
							+ characterUUID);
					this.readValue(bluetoothGattCharacteristic);
				}
			}
		}
	}

	/**
	 * 
	 * @param serviceUUID
	 * @param characteristicUUID
	 */
	public void setNotification(String serviceUUID, String characteristicUUID,
			boolean enable) {
		for (BluetoothGattService bluetoothGattService : bleService
				.getSupportedGattServices(device)) {
			String gattServiceUUID = Long.toHexString(
					bluetoothGattService.getUuid().getMostSignificantBits())
					.substring(0, 4);
			for (BluetoothGattCharacteristic bluetoothGattCharacteristic : bluetoothGattService
					.getCharacteristics()) {
				String characterUUID = Long.toHexString(
						bluetoothGattCharacteristic.getUuid()
								.getMostSignificantBits()).substring(0, 4);
				if (gattServiceUUID.equals(serviceUUID)
						&& characteristicUUID.equals(characterUUID)) {
					this.setCharacteristicNotification(
							bluetoothGattCharacteristic, enable);
				}
			}
		}
	}
	private byte[] stringToByte(String s){
		return  s.getBytes();
	}
}
