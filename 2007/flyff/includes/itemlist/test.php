<?php
$query = mysql_query("SELECT icon FROM itemlist");
while ($sql = mysql_fetch_array($query))
{
 if ($sql[icon])
 {
  $file = @fopen('images/itemlist/'.$sql[icon].'.jpg', 'r');
  if ($file)
  {
   fclose($file);
   $ok++;
  } else {
   echo $sql[icon]."<br />\n";
  }
 } else {
  $empty++;
 }
}
echo "<br />Ennyit ellenõriztem: ".mysql_num_rows($query)."<br />\nEnnyi egyezõ képet találtam: $ok<br />\nEnnyinek nincs ikonja: $empty";
?>
