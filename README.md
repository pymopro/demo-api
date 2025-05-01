```markdown
# API PYMO PRO

**واجهة برمجة التطبيقات للتحكم بوحدات ESP32/ESP8266 عبر السحابة**

---

## الوصف
PYMO PRO API هي واجهة برمجة تطبيقات تُمكّن المستخدمين من التحكم في وحدات ESP32/ESP8266 بشكل فوري عبر الإنترنت باستخدام تقنية WebSocket. توفر هذه الواجهة تحكمًا دقيقًا في إشارات GPIO للوحدات الموصولة، مع دعم كامل لمجموعة واسعة من الدبابيس.

---

## الميزات
- **تحكم فوري**: استخدام WebSocket لنقل الأوامر في الوقت الفعلي.
- **دعم واسع للدبابيس**: تغطية معظم دبابيس ESP32/ESP8266 (مثل `pin5`, `pin22`, `pin33`...).
- **مصادقة آمنة**: استخدام رمز API (Bearer Token) للوصول الآمن.
- **مجاني تمامًا**: لا رسوم متكررة أو قيود على الاستخدام.

---

## المتطلبات الأولية
- وحدة ESP32 أو ESP8266.
- شبكة Wi-Fi للاتصال.
- رمز API خاص (متوفر في حسابك على [PYMO PRO](https://pymo.pro)).
- مكتبتا [`ArduinoWebsockets`](https://github.com/gilmaimon/ArduinoWebsockets) و [`ArduinoJson`](https://arduinojson.org/).

---

## التثبيت
1. **ثبت Arduino IDE** من [الموقع الرسمي](https://www.arduino.cc/).
2. **ثبّت المكتبات المطلوبة** عبر مدير المكتبة (Library Manager):
   - `ArduinoWebsockets`
   - `ArduinoJson`
3. **استورد الملف ZIP** للإصدار [الحالي](https://pymo.pro/download/v1) (إذا كان متاحًا).

---

## الاستخدام

### المصادقة
أضف رمز API كمعلمة استعلام في رابط WebSocket:
```cpp
String ws_url = "ws://api.pymo.pro/ws?token=" + token;
```

### نقاط النهاية
| النقطة | الوصف |
|-------|------|
| `ws://api.pymo.pro/ws` | WebSocket لاستقبال الأوامر من السحابة. |

### هيكل الطلب
يتم إرسال الأوامر بصيغة JSON لتحديد حالة كل دبوس:
```json
{
  "pins": {
    "pin22": 1,
    "pin12": 0,
    "pin13": 1,
    ...
  }
}
```
- القيمة `1`: تشغيل الدبوس (HIGH).
- القيمة `0`: إيقاف الدبوس (LOW).

---

## مثال على التنفيذ (C++)
```cpp
/*
============================================================
مرحبًا بكم في API PYMO PRO.
 يمكنك الحصول على شرح مفصّل عبر 
https://docs.pymo.pro/
بدون قيود، مجاني تمامًا،
 بدون رسوم شهرية 
 https://pymo.pro
============================================================
*/

#include <WiFi.h>
#include <ArduinoWebsockets.h>
#include <ArduinoJson.h>

using namespace websockets;

// تكوين Wi-Fi
const char* ssid     = "YOUR_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

// رمز API
const String token = "YOUR_API_TOKEN";

String ws_url = "ws://api.pymo.pro/ws?token=" + token;
const char* ws_url_cstr = ws_url.c_str();

WebsocketsClient client;

void onMessage(WebsocketsMessage message) {
  Serial.println("Received message:");
  Serial.println(message.data());

  DynamicJsonDocument doc(1024);
  DeserializationError error = deserializeJson(doc, message.data());
  if (error) {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.f_str());
    return;
  }

  JsonObject pins = doc["pins"];
  for (JsonPair kv : pins) {
    int pinNumber = atoi(kv.key().c_str() + 3); // إزالة "pin" من المفتاح
    int pinValue = kv.value().as<int>();
    pinMode(pinNumber, OUTPUT);
    digitalWrite(pinNumber, pinValue);
  }
}

void setup() {
  Serial.begin(115200);

  // الاتصال بشبكة Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(200);
    Serial.print(".");
  }
  Serial.println("WiFi متصل");

  // إعداد WebSocket
  client.onMessage(onMessage);

  // الاتصال بالرابط
  if (client.connect(ws_url)) {
    Serial.println("WebSocket متصل!");
  } else {
    Serial.println("فشل الاتصال بـ WebSocket.");
  }
}

void loop() {
  client.poll();  // استقبال الرسائل الجديدة
}
```

---

## الإصدار الحالي
- **الإصدار**: 1.0.0 (مستقر)
- **تاريخ الإصدار**: 2023

---

## روابط التحميل
| الإصدار | الرابط |
|--------|-------|
| الإصدار 1 | [تحميل الملف ZIP](https://docs.pymo.pro) |

---

## حقوق الملكية
© 2023 PYMO PRO. جميع الحقوق محفوظة.

تصميم وبرمجة: [فريق PYMO PRO](https;//pymo.pro).  
PYMO PRO AI ®
```
