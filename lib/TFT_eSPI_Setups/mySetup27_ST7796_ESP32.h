#define USER_SETUP_ID 27

// Driver ativo
#define ST7796_DRIVER

// Pinagem correta e validada
//#define TFT_MISO 19     // Comente se o display não usar retorno de dados (apenas escrita)
#define TFT_MOSI 23
#define TFT_SCLK 18
#define TFT_CS   15       // Chip select
#define TFT_DC    2       // Data/Command
#define TFT_RST   4       // Reset

// Fontes (mantenha como está)
#define LOAD_GLCD
#define LOAD_FONT2
#define LOAD_FONT4
#define LOAD_FONT6
#define LOAD_FONT7
#define LOAD_FONT8
#define LOAD_GFXFF
#define SMOOTH_FONT

// Frequência SPI corrigida para estabilidade
#define SPI_FREQUENCY       27000000   // 27 MHz é o valor padrão estável para ST7796
#define SPI_READ_FREQUENCY  20000000