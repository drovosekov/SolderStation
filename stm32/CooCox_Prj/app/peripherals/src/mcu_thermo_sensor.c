#include "main.h"


#ifdef SENS_3WIRE
s8 get_temperature_3wire(){
	u8 buf[2];

    if(OW_Send(OW_SEND_RESET, (u8 *)"\xcc\x44", 2, 0, 0, OW_NO_READ) == OW_NO_DEVICE)
    	{return 0;}

#if DS18B20_RESOLUTION == DS18B20_9BIT
    delay_for(100);
#endif
#if DS18B20_RESOLUTION == DS18B20_10BIT
    delay_for(200);
#endif
#if DS18B20_RESOLUTION == DS18B20_11BIT
    delay_for(400);
#endif
#if DS18B20_RESOLUTION == DS18B20_12BIT
    delay_for(750);
#endif

    OW_Send(OW_SEND_RESET, (u8 *)"\xcc\xbe\xff\xff", 4, (u8 *)buf, 2, 2);

	buf[0] >>= 4; // ������� ������� �����
	buf[1] <<= 4; // ������� ������ �����

	return(buf[1] | buf[0]); // ���������� 2 ����� -> �������
}
#endif

#ifdef SENS_2WIRE
//�� �������� �� ������!!!
void get_temperature_2wire(int *hb, int *lb){
	//������� ���� �������� - ��������
    if(OW_Send(OW_SEND_RESET, (u8 *)"\xcc\x44", 2, 0, 0, OW_NO_READ)==OW_NO_DEVICE)
    	{return;}

	//PIN_ON(LED_GREEN);
    // ��������� ������� ������������ ������ - ������ "�������" �� ����
    OW_out_set_as_Power_pin();

    // ����������� ����� ��������� (�������� 750 �� ��� 12-������� ���������)
#if DS18B20_RESOLUTION == DS18B20_9BIT
    delay_for(100);
#endif
#if DS18B20_RESOLUTION == DS18B20_10BIT
    delay_for(200);
#endif
#if DS18B20_RESOLUTION == DS18B20_11BIT
    delay_for(400);
#endif
#if DS18B20_RESOLUTION == DS18B20_12BIT
    delay_for(750);
#endif

    // ��������������� ������� ����������� UART
    OW_out_set_as_TX_pin();

	u8 buf[2];
    OW_Send(OW_SEND_RESET, (u8 *)"\xcc\xbe\xff\xff", 4, (u8 *)buf, 2, 2);

#if 0
	buf[0] >>= 4; // ������� ������� �����
	buf[1] <<= 4; // ������� ������ �����

	return(buf[1] | buf[0]); // ���������� 2 ����� -> �������
#endif

	get_temperature_value(buf, *hb, *lb);
}

void get_temperature_value(u8 *buf, int hb, int lb){
    //���������� ����
    if ((buf[1] & 0x80) != 0) {    //���� ������� ������ �� 0 ��..
    	hb=-hb; //������ ���� "-"
        unsigned int tmp;        //��������� ����������� �������������� � ������
        tmp = ((unsigned int)buf[1]<<8) | buf[0]; //��������� 0 � 1 ���� ��� �������
        tmp = ~tmp + 1; //��������� �������� ����������� ���������
        buf[0] = tmp; //���������� ��������� ��������������
        buf[1] = tmp>>8;
    }
    hb = ((buf[1] & 7)<<4)|(buf[0]>>4);//����� �����
    lb = (buf[0] & 15);//���������� �������
    lb = (lb<<2) + (hb<<3);// �������� �� 10
    lb = (lb>>4);//����� �� 16 ��� �������� �� 0.0625
}
#endif
