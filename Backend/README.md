# home-accounting-backend
Home-accounting ist eine einfache Buchhaltung-Anwendung für den privaten Haushalt. Dieses Repository implementiert die CRUD REST Backend-API + Datenbanksetup für home-accounting via eine Desktop-Anwendung.
## Requirements
Zum Bauen:
- Visual Studio 2022
Zum Ausführen:
- Windows 10 x86_64
- libmicrohttpd .dll Datei in den Ordner der .exe verschieben

## External libraries / Dependencies
- [libmicrohttpd](https://ftpmirror.gnu.org/libmicrohttpd/libmicrohttpd-latest-w32-bin.zip)

## Endpoints
|Endpunkt|HTTP-Verb|Purpose|
|--|--|--|
|`/konten`|GET|Im Moment gibt es ein hello world zurück|