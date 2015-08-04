#/bin/env bash
set -e

cmd='./ex17 db.txt'

$cmd c
$cmd s 1 name email
$cmd g 1
$cmd s 2 name2 email2
$cmd g 2
$cmd d 1
$cmd s 1 name3 email3
$cmd g 1
$cmd l
