<?php
if (is_uploaded_file($_FILES['userfile']['tmp_name']) && $_SESSION[name]) {
 if ($_FILES['userfile']['size'] < 512000) {
  $filename = $_FILES['userfile']['name'];
  $pieces = explode('.', $filename);
  $ext = $pieces[count($pieces)-1];
  if (strtolower($ext) == 'jpg' or strtolower($ext) == 'jpeg') {
   $kepek = mysql_num_rows(mysql_query("SELECT id FROM images WHERE name = '$_SESSION[name]' AND hidden <> '1'"));
   if ($kepek < 500) {
    $file = $_FILES['userfile']['tmp_name'];
    $src = imagecreatefromjpeg($file);
    list($width,$height) = getimagesize($file);
    $newwidth = 100;
    $newheight = ($height/$width)*$newwidth;
    if ($newheight > 100) {
     $width2 = $newwidth;
     $height2 = $newheight;
     $newheight = 100;
     $newwidth = ($width2/$height2)*$newheight;
    }
    $sql = mysql_fetch_array(mysql_query("SELECT id FROM images ORDER BY id DESC LIMIT 1"));
    $id = $sql[id] + 1;
    $kep = imagecreatetruecolor($newwidth,$newheight);
    imagecopyresampled($kep,$src,0,0,0,0,$newwidth,$newheight,$width,$height);
    imagejpeg($kep,'images/gallery/'.$id.'_th.jpg',100);
    move_uploaded_file($file, 'images/gallery/'.$id.'.jpg');
    imagedestroy($src);
    imagedestroy($kep);
    $date = formed_time();
    if ($_POST[hidden]) {
     $hidden = '1';
    }
    mysql_query("INSERT INTO images (id, name, date, hidden) VALUES ('$id', '$_SESSION[name]', '$date', '$hidden')");
    // unlink($_FILES['userfile']['tmp_name']);
    $comment = 'success';
   } else {
    $comment = 'full';
   }
  } else {
   $comment = 'badfile';
  }
 } else {
  $comment = 'bigfile';
 }
} else {
 $comment = 'nofile&error='.$_FILES['userfile']['error'];
}
header('Location: ?p=gallery/gallery&n='.$comment);
?>