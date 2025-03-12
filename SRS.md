# Software Requirements Specification (SRS)

**Projekt:** Smart lampa  
**Version:** 1.0  
**Datum:** 10/3-2025  
**Författare:** Grupp 1!

---

## 1. Introduktion

### 1.1 Syfte
Detta dokument specificerar kraven och funktionaliteten för "Smart Lampa", en intelligent belysningslösning som reagerar på både ljud- och ljusnivåer i dess omgivning.

### 1.2 Målgrupp
Denna SRS riktar sig till utvecklare och produktägare som är involverade i utvecklingen av den smarta lampan, samt undervisare för bedömning.

### 1.3 Omfattning
Smart Lampa är designad för att förbättra inomhusbelysning genom att automatiskt anpassa sig efter rummets förhållanden. Huvudfunktionerna är:
- **Ljudaktivering:** Lampan tänds när ljud detekteras i mörka miljöer.
- **Automatisk ljusstyrka:** Justerar sin ljusstyrka baserat på omgivningens ljusnivå.
- **Flera lägen:** Möjlighet att byta till olika ljuslägen, såsom nattläge och partyläge (planerad för framtida versioner).

---

## 2. Allmän beskrivning

### 2.1 Produktperspektiv
Den smarta lampan är en fristående enhet men kan potentiellt integreras med smarta hem-ekosystem som Google Home eller Alexa.

### 2.2 Produktfunktioner
- **Ljudaktivering:** Lampan tänds vid upptäckt ljud i mörka miljöer.
- **Automatisk ljusstyrka:** Justerar sig efter omgivningens ljusnivå.
- **Färg- och ljuslägen:** Stöd för olika belysningslägen i framtida versioner.

### 2.3 Användningsfall
- **Scenario 1:** En person går in i ett mörkt rum och klappar händerna - lampan tänds automatiskt.
- **Scenario 2:** Ett rum blir mörkare på kvällen - lampans ljusstyrka minskar för att spara energi.
- **Scenario 3:** Användaren vill aktivera ett partyläge där lampan ändrar färg dynamiskt (vid framtida versioner).

---

## 3. Specifika krav

### 3.1 Funktionskrav

| **ID** | **Krav**              | **Beskrivning**                                                      |
|--------|-----------------------|----------------------------------------------------------------------|
| F1     | Ljudaktivering        | Lampan ska tändas vid ljudsignaler i mörka miljöer.                  |
| F2     | Automatisk ljusstyrka | Ljusstyrkan ska justeras efter omgivande ljus.                         |
| F3     | Olika ljuslägen       | Lampan ska kunna byta ljusläge, t.ex. nattläge, partyläge (vid framtida versioner). |

### 3.2 Icke-funktionella krav

| **ID** | **Krav**           | **Beskrivning**                                                              |
|--------|--------------------|------------------------------------------------------------------------------|
| NF1    | Utvecklingsmiljö   | Koden ska skrivas i C++.                                                       |
| NF2    | Hårdvaruplattform  | Lampan ska styras av en Arduino Uno R4 WiFi-enhet.                             |
| NF3    | Responstid         | Lampan ska reagera på ljud och ljus-förändringar inom X sekunder.              |
| NF4    | Energibesparing    | Lampan ska vara optimerad för låg strömförbrukning.                            |
| NF5    | Kompatibilitet     | Lampan ska kunna integreras med smarta hem-system (vid framtida versioner).     |

---

## 4. Begränsningar och Antaganden
- Lampan ska fungera inomhus.
- Vi siktar på strömgivning med likspänning och **INTE** växelström.
- En enkel mikrofon och ljussensor ska räcka för att detektera ljud och ljusförändringar.

---

## 5. Framtida Utvecklingsförslag
- Implementering av en app för att styra lampan via smartphone.
- Möjlighet att schemalägga ljusinställningar.
- Röststyrning via smarta assistenter.
- Stöd för flerfärgade LED-lampor / sammankoppling med flera.

---

## 6. Slutsats
Den smarta lampan är en innovativ lösning som kombinerar bekvämlighet och energieffektivitet. Med en tydlig kravspecifikation och användning av C++ och Arduino kan projektet genomföras på ett strukturerat sätt med framtida expansion i åtanke.
