## Heltec Sensor Board 

Sensors:  
        - DHT22
        - MH-Z14A
        - MH-Z19
        - SGP30
        - Gravity CO2 Sensor V2.0
        - GYML8511
        - CJMCU-811
        
Peripherals: 
        - I2C OLED Display



# Parser-Code:(Language: https://elixir-lang.org/ )

```
############################################################################################################
#
#   Author: Tjark Ziehm ( OHIOH e.V. )
#   name: StadtKielDunkelTurnhallenParser.ex
#   edit: 2021-07-09
#   version: 0.9
#   questions: kontakt@ohioh.de
#   documentation: coming soon
#
############################################################################################################
  # Changelog:
  #   07.09.2021 [TZ]: created the file
  #
  
  defmodule Parser do
    use Platform.Parsing.Behaviour

    def parse(<<state::16,bat::16,temperature::16,humidity::16,cotwo::16,voc::16>>, %{meta: %{frame_port: 2}}) do
      %{
        Status: state,
        Battery: bat,
        Temperature: temperature,
        Humidity: humidity,
        CO2: cotwo,
        VOC: voc,
      }
    end

    def fields do
      [
        %{
          "field" => "Status",
          "display" => "Status"
        },
        %{
          "field" => "Battery",
          "display" => "Batterie-Level",
          "unit" => "%"
        },
                %{
          "field" => "Temperature",
          "display" => "Temperature",
          "unit" => "°C"
        },
                %{
          "field" => "Humidity",
          "display" => "Luftfeuchtigkeit",
          "unit" => "%"
        },
        %{
          "field" => "CO2",
          "display" => "Kohlenstoffdioxid: CO2",
          "unit" => "ppm"
        },
        %{
          "field" => "VOC",
          "display" => "VOC",
          "unit" => "ppb"
        }
      ]
    end

  end

```
