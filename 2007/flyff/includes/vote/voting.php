<?php
if ($_SESSION[name]) {
 $sql = mysql_fetch_array(mysql_query("SELECT * FROM votes WHERE id = '$_GET[id]' LIMIT 1"));
 $answer = $_GET[answer];
 if ($sql['answer'.$answer]) {
  if (substr_count($sql[people], '*'.$_SESSION[name].'*') == 0) {
   $people = $sql[people].'*'.$_SESSION[name].'*';
   $number = $sql['count'.$answer] + 1;
   mysql_query("UPDATE votes SET count".$answer." = '$number', people = '$people' WHERE id = '$_GET[id]' LIMIT 1");
   $comment = 'voted';
  } else {
   $comment = 'cantvote';
  }
 }
 header('Location: ?p=vote/votes&id='.$_GET[id].'&n='.$comment);
}
?>