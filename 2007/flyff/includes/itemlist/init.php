<?php
function ItemType($parts,$weapontype)
{
 switch ($parts)
 {
  case 2: return 'Ruha';
  case 4: return 'Kesztyû';
  case 5: return 'Cipõ';
  case 6: return 'Sapka';
  case 8: return 'Köpeny';
  case 10 :
   switch ($weapontype)
   {
    case 1: return 'Sword';
    case 2: return 'Axe';
    case 3: return 'Stick';
    case 4: return 'Knuckle';
    case 5: return 'Staff';
    case 6: return 'Wand';
    case 20: return 'Yoyo';
    case 21: return 'Bow';
    default : return 'Ismeretlen fegyvertípus';
   }
  case 11: return 'Pajzs';
  case 12: return 'Maszk';
  case 13: return 'Repülõ eszköz';
  case 19: return 'Nyaklánc';
  case 20: return 'Gyûrû';
  case 22: return 'Fülbevaló';
  case 25: return 'Poster vagy arrow';
  case 26: return 'Divat sapka';
  case 27: return 'Divat ruha';
  case 28: return 'Divat kesztyû';
  case 29: return 'Divat cipõ';
  default : return 'Ismeretlen';
 }
}

function Job($job)
{
 switch($job)
 {
  case 0 : return 'Vagrant';
  case 1 : return 'Mercenary';
  case 2 : return 'Acrobat';
  case 3 : return 'Assist';
  case 4 : return 'Magician';
  case 5 : return 'Puppetier';
  case 6 : return 'Knight';
  case 7 : return 'Blade';
  case 8 : return 'Jester';
  case 9 : return 'Ranger';
  case 10 : return 'Ringmaster';
  case 11 : return 'Billposter';
  case 12 : return 'Psykeeper';
  case 13 : return 'Elementor';
  case 14 : return 'Gatekeeper';
  case 15 : return 'Doppler';
  case 16 : return 'Knight MASTER';
  case 17 : return 'Blade MASTER';
  case 18 : return 'Jester MASTER';
  case 19 : return 'Ranger MASTER';
  case 20 : return 'Ringmaster MASTER';
  case 21 : return 'Billposter MASTER';
  case 22 : return 'Psykeeper MASTER';
  case 23 : return 'Elementor MASTER';
  case 24 : return 'Knight HERO';
  case 25 : return 'Blade HERO';
  case 26 : return 'Jester HERO';
  case 27 : return 'Ranger HERO';
  case 28 : return 'Ringmaster HERO';
  case 29 : return 'Billposter HERO';
  case 30 : return 'Psykeeper HERO';
  case 31 : return 'Elementor HERO';
  default : return 'Ismeretlen';
 }
}

function ItemSex($sex)
{
 switch($sex)
 {
  case 0: return 'Férfi';
  case 1: return 'Nõi';
  default : return 'Mindkettõ';
 }
}

function Bonus($bonus)
{
 switch($bonus)
 {
  case 1: return array('STR','');
  case 2: return array('DEX','');
  case 3: return array('INT','');
  case 4: return array('STA','');
  case 5: return array('Yoyo sebzés','');
  case 6: return array('Bow sebzés','');
  case 7: return array('CHR_RANGE','');
  case 8: return array('Távolsági BLOCK','');
  case 9: return array('Critical','%');
  case 10: return array('Bleeding','');
  case 11: return array('Speed','');
  case 12: return array('Atk vagy def (min)','');
  case 13: return array('Atk vagy def (max)','');
  case 14: return array('Közeli BLOCK','');
  case 15: return array('Earth Mastery','');
  case 16: return array('Immobilizálás','');
  case 17: return array('Fire Mastery','');
  case 18: return array('Water Mastery','');
  case 19: return array('Electric Mastery','');
  case 20: return array('Win Mastery','');
  case 21: return array('Knuckle sebzés','');
  case 22: return array('PvP sebzés','');
  case 23: return array('nincs','');
  case 24: return array('Attack speed','');
  case 25: return array('Sword sebzés','');
  case 26: return array('Defense','');
  case 27: return array('Mágia elleni védelem','');
  case 28: return array('Elektromosság elleni védelem','');
  case 29: return array('Reflect damage','');
  case 30: return array('Tûz elleni védelem','');
  case 31: return array('Szél elleni védelem','');
  case 32: return array('Víz elleni védelem','');
  case 33: return array('Föld elleni védelem','');
  case 34: return array('Axe sebzés','');
  case 35: return array('Max HP','');
  case 36: return array('Max MP','');
  case 37: return array('Max FP','');
  case 38: return array('HP','');
  case 39: return array('MP','');
  case 40: return array('FP','');
  case 41: return array('HP recovery rate','');
  case 42: return array('MP recovery rate','');
  case 43: return array('FP recovery rate','');
  case 44: return array('Kill HP','');
  case 45: return array('Kill MP','');
  case 46: return array('Kill FP','');
  case 47: return array('Hit rate','');
  case 48: return array('nincs','');
  case 49: return array('Clear buff','');
  case 50: return array('Steal HP','');
  case 51: return array('Attack Speed','%');
  case 52: return array('Max HP','%');
  case 53: return array('Max MP','%');
  case 54: return array('Max FP','%');
  case 55: return array('WEAEATKCHANGE','');
  case 56: return array('Steal HP','');
  case 57: return array('Stun','%');
  case 58: return array('Auto HP','');
  case 59: return array('Dark','%');
  case 60: return array('Poision','%');
  case 61: return array('Immunitás','');
  case 62: return array('Mágikus sebzés','');
  case 63: return array('Damage','');
  case 64: return array('Státusz','');
  case 65: return array('PARRY','');
  case 66: return array('Attack','%');
  case 67: return array('Exp','');
  case 68: return array('Ugrás','');
  case 69: return array('Steal HP','%');
  case 70: return array('Bleeding','%');
  case 71: return array('Exp recovery','');
  case 72: return array('Defense','%');
  case 73: return array('MP decrease rate','');
  case 74: return array('FP decrease rate','');
  case 75: return array('Spell rate','');
  case 76: return array('Cast critical rate','');
  case 77: return array('Critical sebzés','%');
  case 78: return array('Skill level','');
  case 79: return array('Monster damage','');
  case 80: return array('PvP damage','');
  case 81: return array('Melee steal HP','');
  case 82: return array('Heal','');
  case 83: return array('Attack power','');
  case 85: return array('1H mastery','');
  case 86: return array('2H mastery','');
  case 87: return array('Yoyo mastery','');
  case 88: return array('Bow mastery','');
  case 89: return array('Knuckle mastery','');
  case 90: return array('Hawkeye %','');
  case 91: return array('Mágia ellenállás','%');
  case 92: return array('Giftbox','');
  case 93: return array('ADKPARAMARY','');
  case 10000: return array('Money','');
  case 10001: return array('Pxp','');
  case 10002: return array('Resist all','');
  case 10003: return array('All stat','');
  case 10004: return array('HPDMG_UP','');
  case 10005: return array('Defense és Hit rate -','');
  case 10006: return array('CURE','');
  case 10007: return array('HP recovery rate','');
  case 10008: return array('MP recovery rate','');
  case 10009: return array('FP recovery rate','');
  case 10010: return array('Sebesség és ugrás','');
  case 10011: return array('Mastery all','');
  case 10012: return array('Recovery all','');
  case 10013: return array('Recovery all rate','');
  case 10014: return array('Kill all','');
  case 10015: return array('Kill HP rate','');
  case 10016: return array('Kill MP rate','');
  case 10017: return array('Kill FP rate','');
  case 10018: return array('Kill all rate','');
  case 10019: return array('All decrease rate','');
 }
}

function EchoBonus($dest1,$dest2,$dest3,$adj1,$adj2,$adj3)
{
 if ($dest1 <> -1 && $adj1 <> -1)
 {
  $bonus = Bonus($dest1);
  $result .= $bonus[0].' + '.$adj1.$bonus[1].'<br />';
 }
 if ($dest2 <> -1 && $adj2 <> -1)
 {
  $bonus = Bonus($dest2);
  $result .= $bonus[0].' + '.$adj2.$bonus[1].'<br />';
 }
 if ($dest3 <> -1 && $adj3 <> -1)
 {
  $bonus = Bonus($dest3);
  $result .= $bonus[0].' + '.$adj3.$bonus[1].'<br />';
 }
 return $result;
}



/*

name,itemjob,itemLV,abilitymin,abilitymax,parts,itemsex,handed

*/
?>