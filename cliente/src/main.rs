slint::include_modules!();
use std::io::{Read, Write};
use std::net::{TcpListener, TcpStream};
use std::error::Error;
use std::os::unix::raw::gid_t;
use std::str::from_utf8;
use std::sync::mpsc;
use std::thread;
use std::time::Duration;
use signals2::*;
use slint::Weak;

macro_rules! attempt { // `try` is a reserved keyword
   (@recurse ($a:expr) { } catch ($e:ident) $b:block) => {
      if let Err ($e) = $a $b
   };
   (@recurse ($a:expr) { $e:expr; $($tail:tt)* } $($handler:tt)*) => {
      attempt!{@recurse ($a.and_then (|_| $e)) { $($tail)* } $($handler)*}
   };
   ({ $e:expr; $($tail:tt)* } $($handler:tt)*) => {
      attempt!{@recurse ($e) { $($tail)* } $($handler)* }
   };
}

fn main() -> Result<(), slint::PlatformError> {
    let ui = AppWindow::new()?;
    let ui_clone=ui.as_weak();
    let mut myestado="na";
    let (tx, rx) = mpsc::channel();
    let sig: Signal<()> = Signal::new();
    let sig_clone=sig.clone();

    let _tcpthread=
        thread::Builder::new().name("threadtcp".to_string()).spawn(move||{
        loop {
            thread::sleep(Duration::from_millis(3000));
            match TcpStream::connect("localhost:8085") {
                Ok(mut stream) => {
                    println!("Successfully connected to server in port 8085");

                    let msg = b"Hello!";

                    stream.write(msg).unwrap();
                    println!("Sent Hello, awaiting reply...");

                    let mut data = [0 as u8; 6]; // using 6 byte buffer
                    match stream.read_exact(&mut data) {
                        Ok(_) => {
                            if &data == msg {
                                println!("Reply is ok!");
                            } else {
                                let text = from_utf8(&data).unwrap();
                                println!("Unexpected reply: {}", text);
                            }
                        },
                        Err(e) => {
                            println!("Failed to receive data: {}", e);
                        }
                    }
                },
                Err(e) => {
                    println!("Failed to connect: {}", e);
                }
            }
            println!("Terminated.");
            tx.send("connected").unwrap();
            sig_clone.emit();
        }
    }).expect("TODO: panic message");

    let _ui_edit_thread=
        thread::Builder::new().name("ui_edit_thread".to_string()).spawn({move|| {
            for recived in rx{
                ui_clone.upgrade_in_event_loop(move |ui| ui.set_estado(recived.into())).expect("Changed");
                ui_clone.upgrade_in_event_loop(move |ui| ui.set_visibilidad(false.into())).expect("Changed");
            }
    }}).expect("TODO: panic message");

    ui.run()
}
