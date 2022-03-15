#include "qmobot.h"



uint32_t scanTime = 100; //In 10ms (1000ms)
BLEScan* pBLEScan;

/**
* @brief extend adv report parameters
*/
//typedef struct {
//    esp_ble_gap_adv_type_t event_type;              /*!< extend advertising type */
//    uint8_t addr_type;                              /*!< extend advertising address type */
//    esp_bd_addr_t addr;                             /*!< extend advertising address */
//    esp_ble_gap_pri_phy_t primary_phy;              /*!< extend advertising primary phy */
//    esp_ble_gap_phy_t secondly_phy;                 /*!< extend advertising secondary phy */
//    uint8_t sid;                                    /*!< extend advertising sid */
//    uint8_t tx_power;                               /*!< extend advertising tx power */
//    int8_t rssi;                                    /*!< extend advertising rssi */
//    uint16_t per_adv_interval;                      /*!< periodic advertising interval */
//    uint8_t dir_addr_type;                          /*!< direct address type */
//    esp_bd_addr_t dir_addr;                         /*!< direct address */
//    esp_ble_gap_ext_adv_data_status_t data_status;  /*!< data type */
//    uint8_t adv_data_len;                           /*!< extend advertising data length */
//    uint8_t adv_data[251];                          /*!< extend advertising data */
//} esp_ble_gap_ext_adv_reprot_t;

class MyBLEExtAdvertisingCallbacks: public BLEExtAdvertisingCallbacks {
    void onResult(esp_ble_gap_ext_adv_reprot_t report) {
      if(report.event_type & ESP_BLE_GAP_SET_EXT_ADV_PROP_LEGACY){
        // here we can receive regular advertising data from BLE4.x devices
        Serial.println("BLE4.2");
      } else {
        // here we will get extended advertising data that are advertised over data channel by BLE5 divices
        Serial.printf("Ext advertise: data_le: %d, data_status: %d \n", report.adv_data_len, report.data_status);
      }
    }
};

void setup() {
  Serial.begin(115200);
  Serial.println("Scanning...");

  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan(); //create new scan
  pBLEScan->setExtendedScanCallback(new MyBLEExtAdvertisingCallbacks());
  pBLEScan->setExtScanParams(); // use with pre-defined/default values, overloaded function allows to pass parameters
  delay(1000); // it is just for simplicity this example, to let ble stack to set extended scan params
  pBLEScan->startExtScan(scanTime, 3); // scan duration in n * 10ms, period - repeat after n seconds (period >= duration)
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(2000);
}
#endif // CONFIG_BT_BLE_50_FEATURES_SUPPORTED
