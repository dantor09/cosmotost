<?php
// $param = $_GET['first'];

ini_set('display_errors', 'on');
error_reporting(E_ALL);

$operation_type = $_GET['op_type'];
$fname = "Highscore.txt";

if ($operation_type == "read") {
    // echo "read was passed<br>";


    $myfile = fopen($fname, "r") or die("Unable to open highscores.txt");

    while(! feof($myfile)) {
        echo fgets($myfile);
    }

    fclose($myfile);

} else if ($operation_type == "write") {
    // echo "write was passed";

    $fname = "Highscore.txt";
    $myfile = fopen($fname, "w");

    $top_ten = array($_GET['n1'], $_GET['n2'], $_GET['n3'], $_GET['n4'], $_GET['n5'], $_GET['n6'], $_GET['n7'], $_GET['n8'], $_GET['n9'], $_GET['n10']);
    $gamer_tags = array($_GET['t1'], $_GET['t2'], $_GET['t3'], $_GET['t4'], $_GET['t5'], $_GET['t6'], $_GET['t7'], $_GET['t8'], $_GET['t9'], $_GET['t10']);
    
    
    for ($x = 0; $x < 10; $x++) {
        $line = $gamer_tags[$x] . "\t" . $top_ten[$x] . "\n";
//        echo $line;
//        echo "<br>";
        fwrite($myfile, $line);
        
    }

    echo "done";
    fclose($myfile);
    
}

// example url usage:
// https://cs.csub.edu/~mkausch/3350/cosmo_connect.php?op_type=read
// https://cs.csub.edu/~mkausch/3350/cosmo_connect.php?op_type=write&n1=10&t1=mike&n2=20&t2=mike&n3=30&t3=mike&n4=40&t4=mike&n5=50&t5=mike&n6=60&t6=mike&n7=70&t7=mike&n8=80&t8=mike&n9=90&t9=mike&n10=100&t10=mike


// echo "The end";
?>
