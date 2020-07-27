use rand_core::{RngCore, OsRng};
use std::fs::File;
use std::io::prelude::*;

#[derive(Debug)]
pub struct DcryptError {
    pub message: String
}

pub struct Dcrypt {
    pub key: Vec<[u8; 256]>
}

impl Dcrypt {
    pub fn new() -> Dcrypt {
        // generate new key
        let mut dcrypt: Dcrypt = Dcrypt{
            key: vec![]
        };

        dcrypt.new_key();

        dcrypt
    }

    pub fn new_with_key(key: Vec<u8>) -> Result<Dcrypt,DcryptError> {
        let newkey = Dcrypt::deserialize_key(key);
        match newkey {
            Ok(v) => {
                Ok(Dcrypt{
                    key: v
                })
            },
            Err(e) => Err(DcryptError {message: format!("Error: {}", e.message).to_string()})
        }
    }

    pub fn new_key(&mut self) {
        self.populate_graph();
    }

    pub fn encrypt(&self, bytes: &Vec<u8>) -> Result<Vec<u8>,DcryptError> {
        let mut newdata: Vec<u8> = vec![];

        let randchr = OsRng.next_u32() % 256;
        let mut chr = self.key[randchr as usize][randchr as usize];
        for item in bytes {
            chr = self.key[chr as usize][*item as usize];
            newdata.push(chr as u8);
        }

        newdata.push(randchr as u8);
        
        Ok(newdata)
    }

    pub fn decrypt(&self, bytes: &Vec<u8>) -> Result<Vec<u8>,DcryptError> {
        let mut newdata: Vec<u8> = vec![];
        let mut p: Vec<u8> = bytes.clone();
        let mut chr: u8 = *p.last().unwrap();
        p.remove(p.len() - 1);
        chr = self.key[chr as usize][chr as usize];
        for item in p {
            for j in 0..=255 {
                if self.key[chr as usize][j as usize] == item {
                    newdata.push(j);
                    chr = self.key[chr as usize][j as usize];
                    break;
                }
            }
        }

        Ok(newdata)
    }
    
    pub fn write_to_file(&mut self, path: &str) -> Result<u32,DcryptError>{
        let file = File::create(path);

        match file {
            Err(e) => return Err(DcryptError{message:format!("Could not create file: {}", e).to_string()}),
            Ok(mut file) => {
                match file.write_all(&self.serialize_key()[..]) {
                    Ok(_v) => Ok(1),
                    Err(e) => Err(DcryptError{message:format!("Could not write file: {}", e).to_string()})
                }
            }
        }        
    }

    fn populate_graph(&mut self) {
        let mut graph: Vec<[u8; 256]> = vec![];
        for _ in 0..256 {
            let arr = self.randomize_bytes();   
            graph.push(arr);
        }
        
        self.key = graph;
    }

    fn randomize_bytes(&mut self) -> [u8; 256] {
        let mut narr: Vec<u8> = vec![];
        let mut farr: [u8; 256] = [0u8;256];

        for i in 0..256 {
            narr.push(i as u8);
        }

        loop {
            let randint = OsRng.next_u32();
            let index = (randint % narr.len() as u32) as usize;
            farr[(256-narr.len() as u32) as usize] = narr[index];
            narr.remove(index);
            if narr.len() <= 0 {
                break;
            }
        }

        farr
    }

    fn deserialize_key(bytes: Vec<u8>) -> Result<Vec<[u8; 256]>, DcryptError> {
        if bytes.len() != 65536 {
            return Err(DcryptError{message: "Invalid key length".to_string()})
        }
        
        let mut out: Vec<[u8; 256]> = vec![];

        for i in 0..256 {
            let mut pmut: [u8;256] = [0u8;256];
            for j in 0..256 {
                pmut[255-j] = bytes[(i*256)+j];
            }
            out.push(pmut);
        }        

        Ok(out)
    }

    fn serialize_key(&mut self) -> Vec<u8> {
        let mut out: [u8; 65536] = [0u8;65536];
        for i in 0..256 {
            for j in 0..256 {
                out[(i*256) + 255-j] = self.key[i][j];
            }
        }
        out.to_vec()
    }
}
