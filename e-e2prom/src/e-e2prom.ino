#include <EEPROM.h>

//Define o quanto sera alocado entre 4 e 4096bytes
#define MEM_ALOC_SIZE 8
//Variavel com o numero de boots do ESP
uint8_t boot_num = 0;

void  setup() {

  //Inicia a serial em 115200
  Serial.begin(115200);

  //Imprime informaçoes da memoria do ESP
  //Imprime o chip ID da flash
  Serial.printf("Flash chip ID: %d\n", ESP.getFlashChipId());
  //Imprime em bytes o tamanho da memoria
  Serial.printf("Flash chip size (in bytes): %d\n", ESP.getFlashChipSize());
  //Imprime em hertz a frequencia da memoria flash
  Serial.printf("Flash chip speed (in Hz): %d\n", ESP.getFlashChipSpeed());

  //Aloca o espaco definido na memoria
  EEPROM.begin(MEM_ALOC_SIZE);
  //Le o valor de numero de boots na EEPROM
  boot_num = EEPROM.read(0);
  //Imcrementa o valor do boot
  boot_num++;
  //Escreve a atualizacao do valor
  EEPROM.write(0,boot_num);
  //Finaliza para o valor salvar na memoria
  EEPROM.end();
  //Imprime o valor na serial
  Serial.printf("Boot num: %d\n", boot_num);
}

void loop() {
}
