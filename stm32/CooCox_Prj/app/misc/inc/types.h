#ifndef TYPES
#define TYPES

#ifndef false
#define false	0
#define true	!false
#endif

enum EncoderModes{
	selSolderTemperature = 0,
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

typedef enum SolderingStates SolderingStates;

#endif
