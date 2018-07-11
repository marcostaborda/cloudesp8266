# Protótipo para consultar temperatura com NodeMcu - ESP8266

[Link do Projeto IFRS](http://doc.poalab.net.br/projetos/y8aRrkHPLds6rgyaj)

O projeto foi desenvolvido para a IDE do Arduíno.
A primeira etapa é baixar o código, colocar na Arduíno IDE e tentar rodar verificando se acontece algum erro, no caso será por falta de bibliotecas. 
No final desta etapa, consta alguns links de tutoriais que foram utilizados como referência para o desenvolvimento do código e também links de algumas bibliotecas utilizadas.
Antes de enviar o código para o dispositivo tem que enviar os arquivos da pasta data para ele, para isso consta um link de como instalar essa funcionalidade para enviar os arquivos para o dispositivo.

Por ultimo após o envio dos arquivos e após carregar o código, tem que monitorar a porta serial, nela vai listar o nome da rede que deve se conectar para colocar a senha do wi-fi e também lista o ip que deverá digitar no navegador para poder configurar a conexão do dispositivo com outro wi-fi. Após conectar na nova rede, o dispositivo listará na porta serial o ip que ganhou quando se conectou, para conseguir acessar a configuração de cidade tem que se conectar na mesma rede e usar o http://ip/mudalocal como endereço no navegador, ficando por exemplo: 
```sh
http://200.132.51.233/mudalocal
```
Neste endereço vai ser listado a cidade atual, temperatura e também um texto falando sobre o clima.
![print da tela vista no navegador mobile](http://doc.poalab.net.br/cfs/files/stepImages/R4y8SPZTEau6Gq8cZ?token=eyJhdXRoVG9rZW4iOiIifQ%3D%3D&store=stepImagesStore)

O link do vídeo a seguir mostra o primeiro protótipo sem o RGB e com uns LED's de cores diferentes, estes foram substituídos no projeto final por um LED RGB.
https://player.vimeo.com/video/279533558

# Referências e links uteis

[Programando NodeMcu com Arduino IDE](https://www.filipeflop.com/blog/programar-nodemcu-com-ide-arduino)
[Biblioteca Arduino Json](https://github.com/bblanchon/ArduinoJson)
[Biblioteca WiFiManager](https://github.com/tzapu/WiFiManager)
[Instalação do plugin para upload do arquivo data](https://github.com/esp8266/arduino-esp8266fs-plugin) 
[Exemplo de salvar e ler arquivos .json ](https://github.com/esp8266/Arduino/blob/master/libraries/esp8266/examples/ConfigFile/ConfigFile.ino)
[Código encoding](https://github.com/zenmanenergy/ESP8266-Arduino-Examples/blob/master/helloWorld_urlencoded/urlencode.ino)                
