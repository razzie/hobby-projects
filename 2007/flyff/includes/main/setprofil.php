<?php
if ($_GET[save] == 1 && $_SESSION[name]) {
 $sql = mysql_fetch_array(mysql_query("SELECT * FROM users WHERE name = '$_SESSION[name]'"));
 $password_old = $_POST[pass_regi];
 $password_1st = $_POST[pass_elso];
 $password_2nd = $_POST[pass_masodik];

 if (md5($password_old) == $sql[pass]) {
  if ($password_1st == $password_2nd) {
   if (test($password_1st,3,20) && test($password_2nd,3,20)) {
    $pass = md5($password_1st);
    mysql_query("UPDATE users SET pass = '$pass' WHERE name = '$_SESSION[name]'");
    $comment = 'pwchanged';
   } else {
    $comment = 'badpw';
   }
  } else {
   $comment = 'badpw2';
  }
 } else {
  $comment = 'badpw3';
 }
}
if ($_GET[save] == 2 && $_SESSION[name]) {
 $characters = $_POST[characters];
 if (test2($characters,0,2000)) {
  $characters = addslashes(htmlspecialchars($characters));
  mysql_query("UPDATE users SET characters = '$characters' WHERE name = '$_SESSION[name]'");
  $comment = 'char_ok';
 } else {
  $comment = 'char_fail';
 }
}
if ($_GET[save] == 3 && $_SESSION[name]) {
 $fullname = $_POST[fullname];
 if (test($fullname,0,50)) {
  $fullname = addslashes(htmlspecialchars($fullname));
  mysql_query("UPDATE users SET full_name = '$fullname' WHERE name = '$_SESSION[name]'");
  $comment = 'fullname_ok';
 } else {
  $comment = 'fullname_fail';
 }
}
if ($_GET[save] == 4 && $_SESSION[name]) {
 $mail = $_POST[email];
 if (test3($mail,8,50)) {
  mysql_query("UPDATE users SET email = '$mail' WHERE name = '$_SESSION[name]'");
  $comment = 'mail_ok';
 } else {
  $comment = 'mail_fail';
 }
}
if ($_GET[save] == 5 && $_SESSION[name]) {
 $forumname = $_POST[forum_name];
 if (test($forumname,0,50)) {
  $forumname = addslashes(htmlspecialchars($forumname));
  mysql_query("UPDATE users SET forum_name = '$forumname' WHERE name = '$_SESSION[name]'");
  $comment = 'forumname_ok';
 } else {
  $comment = 'forumname_fail';
 }
}

if ($_GET[save] == 6 && $_SESSION[name]) {
 if (is_uploaded_file($_FILES['userfile']['tmp_name']) && $_GET[e] <> 'del') {
  if ($_FILES['userfile']['size'] < 512000) {
   $filename = $_FILES['userfile']['name'];
   $pieces = explode('.', $filename);
   $ext = $pieces[count($pieces)-1];
   if (strtolower($ext) == 'jpg' or strtolower($ext) == 'jpeg') {
    $file = $_FILES['userfile']['tmp_name'];
    $src = imagecreatefromjpeg($file);
    list($width,$height) = getimagesize($file);
    $newwidth = 80;
    $newheight = ($height/$width)*$newwidth;
    if ($newheight > 80) {
     $width2 = $newwidth;
     $height2 = $newheight;
     $newheight = 80;
     $newwidth = ($width2/$height2)*$newheight;
    }
    $kep = imagecreatetruecolor($newwidth,$newheight);
    imagecopyresampled($kep,$src,0,0,0,0,$newwidth,$newheight,$width,$height);
    imagejpeg($kep,$_FILES['userfile']['tmp_name'].'_thumb',100);
    imagedestroy($src);
    imagedestroy($kep);
    $thumb = addslashes(file_get_contents($_FILES['userfile']['tmp_name'].'_thumb'));
    mysql_query("UPDATE users SET avatar = '{$thumb}' WHERE name = '$_SESSION[name]'");
    $comment = '<div class="green_line">Az új fórum avatár sikeresen feltöltve!</div><br /><br />';
    unlink($_FILES['userfile']['tmp_name'].'_thumb');
    unlink($_FILES['userfile']['tmp_name']);
   } else {
    if (strtolower($ext) == "gif") {
     $file = $_FILES['userfile']['tmp_name'];
     $src = imagecreatefromgif($file);
     list($width,$height) = getimagesize($file);
     $newwidth = 80;
     $newheight = ($height/$width)*$newwidth;
     if ($newheight > 80) {
      $width2 = $newwidth;
      $height2 = $newheight;
      $newheight = 80;
      $newwidth = ($width2/$height2)*$newheight;
     }
     if ($width > 80 or $height > 80) {
      $kep = imagecreate($newwidth,$newheight);
      imagecopyresampled($kep,$src,0,0,0,0,$newwidth,$newheight,$width,$height);
      imagegif($kep,$_FILES['userfile']['tmp_name'].'_thumb',100);
      imagedestroy($src);
      imagedestroy($kep);
      $thumb = addslashes(file_get_contents($_FILES['userfile']['tmp_name'].'_thumb'));
      unlink($_FILES['userfile']['tmp_name'].'_thumb');
     } else {
      $thumb = addslashes(file_get_contents($_FILES['userfile']['tmp_name']));
      unlink($_FILES['userfile']['tmp_name']);
     }
     mysql_query("UPDATE users SET avatar = '{$thumb}' WHERE name = '$_SESSION[name]'");
     $comment = 'avatar_ok';
    } else {
     $comment = 'badavatar';
    }
   }
  } else {
   $comment = 'bigavatar';
  }
 } else {
  if (!$_GET[e]) {
   $comment = 'noavatar';
  }
 }
 if ($_GET[e] == 'del') {
  mysql_query("UPDATE users SET avatar = '' WHERE name = '$_SESSION[name]'");
  $comment = 'delavatar';
 }
}
if ($_GET[save] == 7 && $_SESSION[name]) {
 $signature = $_POST[signature];
 if (test($signature,0,500)) {
  $signature = addslashes(htmlspecialchars($signature));
  mysql_query("UPDATE users SET signature = '$signature' WHERE name = '$_SESSION[name]'");
  $comment = 'sign_ok';
 } else {
  $comment = 'sign_fail';
 }
}
if ($comment) {
 header('Location: ?p=main/options&n='.$comment);
} else {
 header('Location: ?p=main/options');
}
?>