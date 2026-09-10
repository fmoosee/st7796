// Arquivo: lib/TFT_eSPI/TFT_Drivers/ST7796_Init.h

#define TFT_INIT_DELAY 0

// Garante o barramento ativo para envio da sequencia
begin_tft_write();

// 1. Reset por software
writecommand(0x01);
end_tft_write();
delay(150);
begin_tft_write();

// 2. Sair do modo Sleep
writecommand(0x11);
end_tft_write();
delay(150);
begin_tft_write();

// 3. Formato de Pixel: 16-bit RGB565
writecommand(0x3A);
writedata(0x55);
delay(10);

// 4. Inversao de colunas (elimina as listras verticais)
writecommand(0xB4);
writedata(0x01);

// 5. Memory Access Control
writecommand(0x36);
writedata(0x48);

// 6. Desativa inversao
writecommand(0x20);

// 7. Ligar display (Display ON)
writecommand(0x29);
delay(100);

end_tft_write();