<?php
if ($_SESSION[name])
{
 $sql = mysql_fetch_array(mysql_query("SELECT * FROM itemlist WHERE id = '$_GET[id]'"));
 if ($sql[id] == $_GET[id]) {
  echo '<div class="title">'.$sql[name].' adatai</div><br /><br />'."\n".'<table width="100%" cellspacing="0" cellpadding="0"><tr valign="top"><td width="48">';
  $icon_name = strtolower($sql[icon]);
  $icon = @fopen('images/itemlist/'.$icon_name.'.jpg', 'r');
  if ($icon)
  {
   echo '<img src="images/itemlist/'.$icon_name.'.jpg" border="0" alt="" />';
   fclose($icon);
  } else {
   echo '<img src="images/qmark.jpg" border="0" alt="" />';
  }
  echo '</td><td>'."\n";

  include 'init.php';
  echo '<table width="100%" callspacing="0" cellpadding="0">'."\n";

  /*
  if ($_SESSION[rank] > 0)
  {
   echo '<tr><td width="100">Ikon:</td><td>'.$sql[icon].'</td></tr>'."\n";
  }
  */

  echo '<tr><td width="100"><b>Név:</b></td><td>'.$sql[name].'</td></tr>'."\n";
  if ($sql[parts] <> -1)
  {
   if ($sql[itemjob] <> -1)
   {
    echo '<tr><td width="100"><b>Job:</b></td><td>'.Job($sql[itemjob]).'</td></tr>'."\n";
   }
   if ($sql[itemLV] <> -1)
   {
    echo '<tr><td width="100"><b>Szint:</b></td><td>'.$sql[itemLV].'</td></tr>'."\n";
   }
   echo '<tr><td width="100"><b>Típus:</b></td><td>'.ItemType($sql[parts],$sql[weapontype]).'</td></tr>'."\n";
   switch($sql[handed])
   {
    case 1: echo '<tr><td width="100"><b>Fegyvertípus:</b></td><td>Egykezes</td></tr>'."\n"; break;
    case 2: echo '<tr><td width="100"><b>Fegyvertípus:</b></td><td>Kétkezes</td></tr>'."\n"; break;
   }
   if ($sql[itemsex] <> -1)
   {
    echo '<tr><td width="100"><b>Nem:</b></td><td>'.ItemSex($sql[itemsex]).'</td></tr>'."\n";
   }
   if ($sql[abilitymin] <> -1 or $sql[abilitymax] <> -1)
   {
    if ($sql[parts] == 10)
    {
     echo '<tr><td width="100"><b>Támadás:</b></td><td>'.$sql[abilitymin].' ~ '.$sql[abilitymax].'</td></tr>'."\n";
    } else {
     echo '<tr><td width="100"><b>Védelem:</b></td><td>'.$sql[abilitymin].' ~ '.$sql[abilitymax].'</td></tr>'."\n";
    }
   }
  }
  $bonus = EchoBonus($sql[destparam1],$sql[destparam2],$sql[destparam3],$sql[adjparamval1],$sql[adjparamval2],$sql[adjparamval3]);
  if ($bonus <> '')
  {
   echo '<tr><td width="100"><b>Bónusz:</b></td><td>'.$bonus.'</td></tr>'."\n";
  }
  echo '<tr><td width="100"><b>Bolti ár:</b></td><td>'.$sql[cost].'</td></tr>'."\n";
  $npc_price = floor($sql[cost]/4);
  if ($npc_price == 0) { $npc_price = 1; }
  echo '<tr><td width="100"><b>Eladási ár:</b></td><td>'.$npc_price.'</td></tr>'."\n";
  if ($sql[comment] <> '')
  {
   echo '<tr><td width="100"><b>Leírás:</b></td><td>'.$sql[comment].'</td></tr>'."\n";
  }

  echo '</table>'."\n".'</td></tr></table>';
 } else {
 echo '<div class="title">Tárgy adatai</div><br /><br /><div class="red_line">Nincs ilyen tárgy.</div>';
 }
} else {
 echo '<div class="title">Tárgy adatai</div><br /><br /><div class="red_line">A megtekintéshez be kell jelentkezned.</div>';
}
?>