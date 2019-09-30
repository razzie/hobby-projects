<div class="title">Tárgylista</div><br />
<p align="justify">
Írd be a keresõbe a keresett tárgy nevét vagy nevének egy részletét, majd a keresõ listázza a találatokat (normál felhasználóknál maximum 10-et). A találatok részleteinek megtekintéséhez regisztrált tagnak kell lenned és be kell jelentkezned.
<br /><br />
<form action="?p=itemlist/search" method="POST">
<b>Kulcsszó:</b><br /><br />
<input type="text" name="search" id="search" onBlur="test('search',3,50)" style="width: 150px" />
<input type="submit" class="button" value="Keresés" />
</from></p>
<br /><br />
<?php
if ($_POST['search'])
{
 $key = $_POST['search'];
} else {
 $key = $_GET['search'];
}
if ($key) {
 if (test($key,3,50))
 {
  $key = htmlspecialchars(addslashes($key));
  if ($_SESSION[rank] > 0 && $_GET[show])
  {
   $query = mysql_query("SELECT id,name,icon FROM itemlist WHERE name LIKE '%$key%' LIMIT $_GET[show]");
  } else {
   $query = mysql_query("SELECT id,name,icon FROM itemlist WHERE name LIKE '%$key%' LIMIT 10");
  }
  $rows = mysql_num_rows(mysql_query("SELECT id FROM itemlist WHERE name LIKE '%$key%'"));
  if ($rows > 0)
  {
   while ($sql = mysql_fetch_array($query))
   {
    $icon_name = strtolower($sql[icon]);
    $icon = @fopen('images/itemlist/'.$icon_name.'.jpg', 'r');
    if ($icon)
    {
     echo '<a href="?p=itemlist/view&amp;id='.$sql[id].'" class="link"><img src="images/itemlist/'.$icon_name.'.jpg" width="16" height="16" border="0" alt="" /> '.$sql[name].'</a><br />'."\n";
     fclose($icon);
    } else {
     echo '<a href="?p=itemlist/view&amp;id='.$sql[id].'" class="link"><img src="images/qmark.jpg" width="16" height="16" border="0" alt="" /> '.$sql[name].'</a><br />'."\n";
    }
   }
   if (!$_GET[show] && $_SESSION[rank] > 0)
   {
    echo '<a href="?p=itemlist/search&amp;search='.$key.'&amp;show='.$rows.'" class="date">Az összes mutatása..</a><br />';
   }
  } else {
   echo '<div class="red_line">Nincs találat.</div>';
  }
 } else {
  echo '<br /><div class="red_line">A megadott kulcsszó nem megfelelõ!</div><br />';
 }
}
?>
