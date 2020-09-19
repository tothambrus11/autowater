String escapeHtml(String data) {
    String buffer = "";
    for(size_t pos = 0; pos < data.length(); ++pos) {
        switch(data[pos]) {
            case '&':  buffer.concat("&amp;");       break;
            case '\"': buffer.concat("&quot;");      break;
            case '\'': buffer.concat("&apos;");      break;
            case '<':  buffer.concat("&lt;");        break;
            case '>':  buffer.concat("&gt;");        break;
            default:   buffer.concat(data[pos]);     break;
        }
    }
    return buffer;
}

void routeIndex(){
    server.on("/", []() {
        if(!requestAuthentication()){
            return;
        }
        String response = u8R"(<html>
<head>
    <style>
        * {
            font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, Oxygen, Ubuntu, Cantarell, 'Open Sans', 'Helvetica Neue', sans-serif;
        }

        html,
        body {
            padding: 15px;
            margin: 0;
        }

        #timingfs label {
            display: block;
        }

        #timeinput {
            border: 1px solid #333;
            display: inline-block !important;
        }

        #timeinput input {
            border: none;
            -moz-appearance: textfield;
            width: 25px;
            padding: 5px;
            font-family: 'Courier New', Courier, monospace;
        }

        #timeinput input:focus {
            outline: none;
        }

        #timeinput input::-webkit-outer-spin-button,
        #timeinput input::-webkit-inner-spin-button {
            -webkit-appearance: none;
            margin: 0;
        }

        input[type=submit] {
            border: 1px solid #333;
            background-color: transparent;
            padding: 5px 10px;
            cursor: pointer;
            margin-top: 10px;
        }

        #container {
            max-width: 600px;
            margin: 0 auto;
        }
    </style>
    <meta charset="utf8">
</head>

<body>
    <div id='container'>
        <h1 style='text-align: center;'>Autowater beállítások</h1>
        <form action='/change-interval' method='GET'>
            <fieldset id='timingfs'>
                <legend>
                    <h3 style='margin:0'>Időzítés</h3>
                </legend>
                Időpont: <label id='timeinput'>
                    <input type='number' min='0' max='23' name='wh' required value=')" + String(settings.watering_time_hours) + u8R"('> :
                    <input type='number' min='0' max='59' name='wm' required value=')" + String(settings.watering_time_minutes) + u8R"('>
                </label>
                <label>
                    <input type='checkbox' name='day' value='0')" + (settings.watering_days[0] ? " checked" : "") + u8R"(>
                    Hétfő
                </label>
                <label>
                    <input type='checkbox' name='day' value='1')" + (settings.watering_days[1] ? " checked" : "") + u8R"(>
                    Kedd
                </label>
                <label>
                    <input type='checkbox' name='day' value='2')" + (settings.watering_days[2] ? " checked" : "") + u8R"(>
                    Szerda
                </label>
                <label>
                    <input type='checkbox' name='day' value='3')" + (settings.watering_days[3] ? " checked" : "") + u8R"(>
                    Csütörtök
                </label>
                <label>
                    <input type='checkbox' name='day' value='4')" + (settings.watering_days[4] ? " checked" : "") + u8R"(>
                    Péntek
                </label>
                <label>
                    <input type='checkbox' name='day' value='5')" + (settings.watering_days[5] ? " checked" : "") + u8R"(>
                    Szombat
                </label>
                <label>
                    <input type='checkbox' name='day' value='6')" + (settings.watering_days[6] ? " checked" : "") + u8R"(>
                    Vasárnap
                </label>
                <label>
                    Öntözés hossza:
                    <input style='width: 60px;' type='number' name='wtime' value=')" + String(((float) settings.watering_duration) / 1000) + u8R"(' step='0.5' min=0 max=3600>
                    másodperc
                </label>
                <input type='submit' value='Mentés'>
            </fieldset>
        </form>
        <form action='/change-wifi' method='GET'>
            <fieldset>
                <legend>
                    <h3 style='margin:0'>WiFi</h3>
                </legend>

                <p style='margin: 3px 0;'>Csatlakozás ehhez a WiFi-hez:</p>
                <table>
                    <tr>
                        <td>
                            <label for='ssid'>SSID:</label>
                        </td>
                        <td>
                            <input type='text' name='wifiSSID' id='ssid' value=')" + escapeHtml(realString(settings.wifiSSID)) + u8R"('>
                        </td>
                    </tr>
                    <tr>
                        <td>
                            <label for='pwd'>Jelszó:</label>
                        </td>
                        <td>
                            <input type='password' placeholder='***' name='wifiPWD'>
                        </td>
                    </tr>
                </table>
                <input type='submit' value='Csatlakozás'>

            </fieldset>
        </form>
        <footer style='text-align: center;'>&copy; 2020 Tóth Ambrus, <a href='https://johetajava.hu' target='_blank'>Jöhet a Java</a></footer>
    </div>

</body>

</html>)";
        server.send(200, "text/html", response);
        Serial.println(response);
    });
}

