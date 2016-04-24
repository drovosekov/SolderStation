#ifndef TYPES
#define TYPES

#ifndef false
#define false	0
#define true	!false
#endif

enum EncoderModes{
	selRealTimeInfo = 0,
	selSolderTemperature,
	selAirFlowPower,
	selAirHeaterTemperature
};

enum SolderingStates{
	notReady = 0,	//��� ��������� - �� �����
	isOff,			//��������
	isOn,			//�������
	isSleepMode,	//��������� ����� ��������
	isPreOn			//�� ��������� - ����� � ���������. ��������� ��� ������ � ���������
};

typedef enum SolderingStates States;

typedef struct SLD_INFO{
	u16 auto_off;		//������� ��������������
	u16 temp;			//���������� ���. �����������
	States state;		//������� ���������
	u8 air_flow;		//����� ������� (������ ��� ����)
}SLD_INFO;

#endif
