<?php
/* 

Usage:

$dc = new dcrypt();
if(file_exists("keyfile")) {
	// Read previously saved keyfile.
	$dc->MakeWithOldKey("keyfile");
} else {
	// Save keyfile.
	$dc->OutputKey("keyfile");
}
$encrypted = $dc->EncryptString("This is a test");
echo $encrypted."\n";
$decrypted = $dc->DecryptString($encrypted);
echo $decrypted."\n";

*/

define('HEIGHT', 256);
define('WIDTH', 256);
define('MAXSIZE', 65536);

class dcrypt {
	private $key;
	
	public function __construct() {
		$this->NewKey();
	}
	
	public function MakeWithOldKey($keyfile) {
		$file = fopen($keyfile, 'rb');
		if(!$file) return false;
		
		$bytes = fread($file, filesize($keyfile));
		fclose($file);
			
		$this->key = $this->deserializeKey($bytes, MAXSIZE);
	}
	
	public function NewKey() {
		$this->key = $this->populateGraph();
	}
	
	public function OutputKey($filename) {
		$key = $this->serializeKey();
		$file = fopen($filename, 'wb');
		if(!$file) {
			return false;
		}
		fwrite($file, $key);
		fclose($file);
		return true;
	}
	
	public function EncryptString($string) {
		return base64_encode($this->Encrypt($string, 0, strlen($string)));
	}
	
	public function Encrypt($data, $offset, $count) {
		$edata = "";
		$y = 0;
		$randchr = mt_rand(0, 255);
		$chr = $this->key[$randchr][$randchr];
		for($i = $offset; $i < $count; $i++) {
			$chr = $this->key[$chr][ord($data[$i])];
			$edata .= pack('C', $chr);
		}
		$edata[$count] = pack('C', $randchr);
		return $edata;
	}
	
	public function DecryptString($string) {
		$str = base64_decode($string);
		$cstr = $this->Decrypt($str, 0);
		$nstr = "";
		for ($i = 0; $i < count($cstr); $i++) {
			$nstr .= chr($cstr[$i]);
		}
		return $nstr;
	}
	
	public function Decrypt($data, $offset) {
		$data = unpack('C*', $data);
		unset($data[0]);
		$data = array_values($data);
		$count = count($data);
		
		$output = array();
		$chr = $data[$count - 1];
		$xchr = 0;
		$chr = $this->key[$chr][$chr];
		for($i = $offset; $i < $count-1; $i++) {
			$xchr = $data[$i];
			for($j = 0; $j < WIDTH; $j++) {
				if($this->key[$chr][$j] == $xchr) {
					$output[$i] = $j;
					$chr = $this->key[$chr][$j];
					break;
				}
			}
		}
		return $output;
	}
	
	private function serializeKey() {
		$data = array();
		$p = 0;
		for($i = 0; $i < HEIGHT; $i++) {
			$xd = $this->key[$i];
			$arr = array();
			for($j = 0; $j < WIDTH; $j++) {
				$arr[$j] = $xd[$j];
			}
			
			$arr = $this->reverse($arr, WIDTH);
			for($j = 0; $j < WIDTH; $j++) {
				$data[$p][$j] = $arr[$j];
			}
			$p++;
		}
		// loop through and create a "string" if you will.
		$ndata = "";
		for ($i = 0; $i < HEIGHT; $i++) {
			for($j = 0; $j < WIDTH; $j++) {
				$ndata .= pack('C', $data[$i][$j]);
			}
		}
		return $ndata;
	}
	
	private function deserializeKey($data, $count) {
		if ($count != MAXSIZE) return false;
		$data = unpack('C*', $data);
		unset($data[0]);
		$data = array_values($data);
		$p = 0;
		$key = array(array());
		for($i = 0; $i < HEIGHT; $i++) {
			$buff = array();
			for ($j = 0; $j < WIDTH; $j++) {
				$buff[$j] = $data[$p];
				$p++;
			}
			$nbuf = $this->reverse($buff, WIDTH);
			for($j = 0; $j < WIDTH; $j++) {
				$key[$i][$j] = $nbuf[$j];
			}
		}
		return $key;
	}
	
	private function populateGraph() {
		$key = array(array());
		for($i = 0; $i < HEIGHT; $i++) {
			$bytes = $this->fillBytes(WIDTH);
			$rbytes = $this->rearrangeBytes($bytes, WIDTH);
			for($j = 0; $j < WIDTH; $j++) {
				$key[$i][$j] = $rbytes[$j];
			}
		}
		return $key;
	}
	
	private function rearrangeBytes($data, $count) {
		$x = array();
		$i = 0;
		while(true) {
			$p = $data[mt_rand(0, 255)];
			if (($i - $count) == 0) {
				$x[$i] = $p;
				$i++;
				continue;
			}
			if(!in_array($p, $x)) {
				$x[$i] = $p;
				$i++;
			}
			if ($i == $count) break;
		}
		return $x;
	}
	
	private function fillBytes($count) {
		$arr = array();
		for($i = 0; $i < $count; $i++) {
			$arr[$i] = $i;
		}
		return $arr;
	}
	
	private function reverse($data, $count) {
		$bytes = array();
		for($i = 0; $i < $count; $i++) {
			$bytes[$i] = $data[$count - $i - 1];
		}
		return $bytes;
	}
}