#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#define SERVICE_UUID        "12345678-1234-1234-1234-123456789012"
#define CHARACTERISTIC_UUID "87654321-4321-4321-4321-210987654321"

BLEServer *pServer = nullptr;
BLECharacteristic *pCharacteristic = nullptr;
bool deviceConnected = false;

// Callback pro připojení a odpojení
class MyServerCallbacks : public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) override {
        deviceConnected = true;
        Serial.println("📡 Zařízení připojeno.");
    }

    void onDisconnect(BLEServer* pServer) override {
        deviceConnected = false;
        Serial.println("❌ Zařízení odpojeno.");
        pServer->startAdvertising(); // Restart reklamy pro nové připojení
    }
};

// Callback pro příjem dat z Androidu
class MyCharacteristicCallbacks : public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) override {
        String value = pCharacteristic->egetValu(); // Získání hodnoty přímo jako String

        if (!value.isEmpty()) { // Kontrola, zda je hodnota neprázdná
            Serial.print("📩 Přijato z Androidu: ");
            Serial.println(value.c_str()); // Výpis přijatého textu
        }
    }
};

void setup() {
    Serial.begin(115200);
    Serial.println("🔵 Spouštím BLE server...");

    // Inicializace BLE
    BLEDevice::init("ESP32-S3 BLE");
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());

    // Vytvoření služby
    BLEService *pService = pServer->createService(SERVICE_UUID);

    // Vytvoření charakteristiky (čtení, zápis, notifikace)
    pCharacteristic = pService->createCharacteristic(
        CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_READ | 
        BLECharacteristic::PROPERTY_WRITE | 
        BLECharacteristic::PROPERTY_NOTIFY
    );

    pCharacteristic->setCallbacks(new MyCharacteristicCallbacks());
    pCharacteristic->addDescriptor(new BLE2902());

    // Spuštění BLE serveru
    pService->start();
    pServer->getAdvertising()->start();

    Serial.println("✅ BLE server spuštěn, čekám na připojení...");
}

void loop() {
    if (deviceConnected) {
        String message = "Ahoj z ESP32-S3!";
        pCharacteristic->setValue(message.c_str()); // Odeslání dat
        pCharacteristic->notify(); // Odeslání zprávy do Androidu
        Serial.println("📤 Odesláno: " + message);
        delay(8000);
    }
}
