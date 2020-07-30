# Respaldo de estados

## Definiciones
- `BACKUP_MAXNUMREGPERFILE`: 15600 -> `13[h]*60*[m/h]*60[s/m]/3[sa]`
- `BACKUP_MAXNUMFILES`: 40
- `BACKUP_DIR_NAME`: "frames"

## Descripción y comportamiento
- El respaldo se realiza en archivos de un dispositivo de almacenamiento 
  masivo USB, utilizando el sistema de archivos fatfs

- Cada archivo de respaldo almacena hasta `BACKUP_MAXNUMREGPERFILE` estados 
  (registros).
  Al menos 1 día de trabajo promedio (~13hs). Alcanzado este límite se crea 
  un archivo nuevo o se recicla el más antiguo. Reciclar un archivo implica 
  truncarlo, sobreescribirlo y adoptarlo como el actual

- El registro del archivo de respaldo es de formato `GStatus` (binario). 

- El programa `frameconv` transforma los archivos de respaldo en tramas.
  `$./frameconv -i <imei> <backup-file>`

- Existe como máximo `BACKUP_MAXNUMFILES` archivos en el directorio. 
  Al menos 40 días. Alcanzado este límite se recicla el archivo más antiguo

- El nombre del archivo se compone de un número incremental (de 0 a 65535) y 
  una extensión `.frm`, es decir `<number>.frm`

- El archivo más nuevo es aquel con número más grande en su nombre

- Los archivos se almacenan en un directorio particular de nombre 
  `BACKUP_DIR_NAME`

- Después del startup se determina el archivo más nuevo, este se adopta como 
  el archivo actual, donde se almacenarán los estados. Si alcanza su valor 
  máximo de registros entonces se crea uno nuevo o se recicla el más antiguo
