# Composición de trama con posición inválida

El envío de una trama con posición inválida transporte información 
adicional, codificada en ASCII, en ciertos campos de la posición. 
A continuación se listan dichos campos y el tipo de info que transportan.

Position Field | Information | Description
------------ | ------------- | -------------
speed | Version code | Versión de código definida por la constante
                       `AGROIOT_VERSION`
date | Release date | Fecha de liberación definida por la constante 
                      `AGROIOT_RELEASE_DATE`
course | Backup status | Estado del módulo de respaldo de estados,
                         definido por el enumerado `BackupCode`
latInd | Last reset source | Origen del último reset, definido por el 
                             enumerado `ResetSource_t`
