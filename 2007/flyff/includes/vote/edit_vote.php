<?php
if ($_SESSION[rank] > 0 && $_GET[id] > 0) {
 $title = addslashes(htmlspecialchars($_POST[title]));
 $answer1 = addslashes(htmlspecialchars($_POST[answer1]));
 $answer2 = addslashes(htmlspecialchars($_POST[answer2]));
 $answer3 = addslashes(htmlspecialchars($_POST[answer3]));
 $answer4 = addslashes(htmlspecialchars($_POST[answer4]));
 $answer5 = addslashes(htmlspecialchars($_POST[answer5]));
 $answer6 = addslashes(htmlspecialchars($_POST[answer6]));
 $answer7 = addslashes(htmlspecialchars($_POST[answer7]));
 $answer8 = addslashes(htmlspecialchars($_POST[answer8]));
 if ($answer1) { $answers += 1; }
 if ($answer2) { $answers += 1; }
 if ($answer3) { $answers += 1; }
 if ($answer4) { $answers += 1; }
 if ($answer5) { $answers += 1; }
 if ($answer6) { $answers += 1; }
 if ($answer7) { $answers += 1; }
 if ($answer8) { $answers += 1; }
 if (test($title,10,300) && test($answer1,0,100) && test($answer2,0,100) && test($answer3,0,100) && test($answer4,0,100) && test($answer5,0,100) && test($answer6,0,100) && test($answer7,0,100) && test($answer8,0,100) && $answers > 1) {
  $time = formed_time();
  mysql_query("UPDATE votes SET title = '$title', answer1 = '$answer1', answer2 = '$answer2', answer3 = '$answer3', answer4 = '$answer4', answer5 = '$answer5', answer6 = '$answer6', answer7 = '$answer7', answer8 = '$answer8', date = '$time' WHERE id = '$_GET[id]'");
  header('Location: ?p=vote/votes&id='.$_GET[id].'&n=edit');
 } else {
  header('Location: ?p=vote/edit&id='.$_GET[id].'&n=fail');
 }
}
?>