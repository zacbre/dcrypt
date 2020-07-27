#dcrypt for Rust 
```
fn main() {
    let mut dcrypt = Dcrypt::new();
    let input: Vec<u8> = vec![1,2,3,4,5,6,7,8,9,10];
    println!("input: {:?}", input);
    let output: Vec<u8> = dcrypt.encrypt(&input).expect("can't encrypt");
    println!("output: {:?}", output);
    let decrypted: Vec<u8> = dcrypt.decrypt(&output).expect("can't decrypt!");
    println!("built in dec: {:?}", decrypted);

    dcrypt.write_to_file("./test.key");

    let mut x: Vec<u8> = match read_a_file() {
        Ok(v) => v,
        Err(e) => panic!(e)
    };
    let dc1 = Dcrypt::new_with_key(x).expect("can't read key!");
    let decrypted1 = dc1.decrypt(&output).expect("can't decrypt!");
    println!("read key dec: {:?}", decrypted1);
}

fn read_a_file() -> std::io::Result<Vec<u8>> {
    let mut file = File::open("./test.key").expect("Unable to open!");

    let mut data = Vec::new();
    file.read_to_end(&mut data);

    return Ok(data);
}
```