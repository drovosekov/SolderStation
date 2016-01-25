//------------------------------------------------------------------------------
// This is Open source software. You can place this code on your site, but don't
// forget a link to my YouTube-channel: https://www.youtube.com/channel/UChButpZaL5kUUl_zTyIDFkQ
// Это программное обеспечение распространяется свободно. Вы можете размещать
// его на вашем сайте, но не забудьте указать ссылку на мой YouTube-канал 
// "Электроника в объектике" https://www.youtube.com/channel/UChButpZaL5kUUl_zTyIDFkQ
// Автор: Надыршин Руслан / Nadyrshin Ruslan
//------------------------------------------------------------------------------
#include "cp1251toepson.h"

#ifdef SOURCE_CODEPAGE
//==============================================================================
// Процедура подменяет символы кириллицы в кодировке 1251 на символы в кодироке 
// EPSON для hd44780 
//==============================================================================
void cp1251_to_epson_convert(char *str)
{
	#if (SOURCE_CODEPAGE == CP1251)
  while (*str != '\0')
  {
    switch (*str)
    {
    case 'А': *str = 0x41; break;      // А
    case 'Б': *str = 0xA0; break;      // Б
    case 'В': *str = 0x42; break;      // В
    case 'Г': *str = 0xA1; break;      // Г
    case 'Д': *str = 0xE0; break;      // Д
    case 'Е': *str = 0x45; break;      // Е
    case 'Ё': *str = 0xA2; break;      // Ё
    case 'Ж': *str = 0xA3; break;      // Ж
    case 'З': *str = 0x33; break;      // З
    case 'И': *str = 0xA5; break;      // И
    case 'Й': *str = 0xA6; break;      // Й
    case 'К': *str = 0x4B; break;      // К
    case 'Л': *str = 0xA7; break;      // Л
    case 'М': *str = 0x4D; break;      // М
    case 'Н': *str = 0x48; break;      // Н
    case 'О': *str = 0x4F; break;      // О
    case 'П': *str = 0xA8; break;      // П
    case 'Р': *str = 0x50; break;      // Р
    case 'С': *str = 0x43; break;      // С
    case 'Т': *str = 0x54; break;      // Т
    case 'У': *str = 0xA9; break;      // У
    case 'Ф': *str = 0xAA; break;      // Ф
    case 'Х': *str = 0x58; break;      // Х
    case 'Ц': *str = 0xE1; break;      // Ц
    case 'Ч': *str = 0xAB; break;      // Ч
    case 'Ш': *str = 0xAC; break;      // Ш
    case 'Щ': *str = 0xE2; break;      // Щ
    case 'Ъ': *str = 0xAD; break;      // Ъ
    case 'Ы': *str = 0xAE; break;      // Ы
    case 'Ь': *str = 0x62; break;      // Ь
    case 'Э': *str = 0xAF; break;      // Э
    case 'Ю': *str = 0xB0; break;      // Ю
    case 'Я': *str = 0xB1; break;      // Я
    case 'а': *str = 0x61; break;      // а
    case 'б': *str = 0xB2; break;      // б
    case 'в': *str = 0xB3; break;      // в
    case 'г': *str = 0xB4; break;      // г
    case 'д': *str = 0xE3; break;      // д
    case 'е': *str = 0x65; break;      // е
    case 'ё': *str = 0xB5; break;      // ё
    case 'ж': *str = 0xB6; break;      // ж
    case 'з': *str = 0xB7; break;      // з
    case 'и': *str = 0xB8; break;      // и
    case 'й': *str = 0xB9; break;      // й
    case 'к': *str = 0xBA; break;      // к
    case 'л': *str = 0xBB; break;      // л
    case 'м': *str = 0xBC; break;      // м
    case 'н': *str = 0xBD; break;      // н
    case 'о': *str = 0x6F; break;      // о
    case 'п': *str = 0xBE; break;      // п
    case 'р': *str = 0x70; break;      // р
    case 'с': *str = 0x63; break;      // с
    case 'т': *str = 0xBF; break;      // т
    case 'у': *str = 0x79; break;      // у
    case 'ф': *str = 0xE4; break;      // ф
    case 'х': *str = 0x78; break;      // х
    case 'ц': *str = 0xE5; break;      // ц
    case 'ч': *str = 0xC0; break;      // ч
    case 'ш': *str = 0xC1; break;      // ш
    case 'щ': *str = 0xE6; break;      // щ
    case 'ъ': *str = 0xC2; break;      // ъ
    case 'ы': *str = 0xC3; break;      // ы
    case 'ь': *str = 0xC4; break;      // ь
    case 'э': *str = 0xC5; break;      // э
    case 'ю': *str = 0xC6; break;      // ю
    case 'я': *str = 0xC7; break;      // я
    }
    str++;
  }
#endif
}
//==============================================================================
#endif
