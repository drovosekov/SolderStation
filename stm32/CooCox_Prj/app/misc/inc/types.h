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
	notReady = 0,	//вне подставки - не готов
	isOff,			//выключен
	isOn,			//включен
	isSleepMode,	//экономный режим ожидания
	isPreOn			//на подставке - готов к включению. Включится при снятии с подставки
};

typedef enum SolderingStates States;

typedef struct SLD_INFO{
	u16 auto_off;		//счетчик автоотключения
	u16 temp;			//переменная уст. температуры
	States state;		//текущее состояние
	u8 air_flow;		//поток воздуха (только для фена)
}SLD_INFO;

#endif
