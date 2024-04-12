slint::include_modules!();
use std::io::{Read, Write};
use std::net::{TcpStream};
use std::error::Error;
use std::ops::Index;
use std::io::{BufRead};
use std::str::{from_utf8, from_utf8_mut};
use std::sync::mpsc;
use std::thread;
use std::time::Duration;
use json::{JsonValue, value};
use ini::Ini;
extern crate ini;
use log::*;
use glog::Flags;

/// Macro `attempt` para realizar intentos de ejecución y manejo de errores de manera concisa.
///
/// Esta macro facilita el manejo de errores en Rust mediante un estilo similar al `try-catch` de otros lenguajes.
/// Se puede usar para encadenar operaciones que pueden fallar y manejar los errores resultantes de manera uniforme.
///
/// La macro `attempt` toma una serie de bloques de código, cada uno de los cuales representa una operación que puede fallar.
/// Si una operación falla, se ejecuta el bloque `catch` asociado.
/// Se puede especificar un número arbitrario de bloques `catch`.
/// El bloque `catch` debe ser seguido por un identificador que represente el error capturado.
/// Cada bloque de código puede contener múltiples expresiones separadas por punto y coma (`;`).
/// Dentro de cada bloque, se puede usar la palabra clave `try` para realizar operaciones que pueden fallar.
/// La macro `attempt` devuelve un resultado `Result<(), Error>` que indica si hubo un error durante la ejecución.
macro_rules! attempt {
    // Regla para el caso base donde no hay más operaciones que realizar
    (@recurse ($a:expr) { } catch ($e:ident) $b:block) => {
        // Maneja el error utilizando el identificador especificado y el bloque proporcionado
        if let Err($e) = $a $b
    };
    // Regla para encadenar una operación con la siguiente y posiblemente manejar un error
    (@recurse ($a:expr) { $e:expr; $($tail:tt)* } $($handler:tt)*) => {
        // Encadena la operación actual con la siguiente y continúa el bucle
        attempt!{@recurse ($a.and_then (|_| $e)) { $($tail)* } $($handler)*}
    };
    // Regla de entrada para la macro `attempt`
    ({ $e:expr; $($tail:tt)* } $($handler:tt)*) => {
        // Inicia el bucle de encadenamiento de operaciones y manejo de errores
        attempt!{@recurse ($e) { $($tail)* } $($handler)* }
    };
}

/// Función principal que ejecuta la lógica principal de la aplicación.
///
/// Esta función configura la interfaz de usuario, establece la comunicación con un servidor TCP,
/// maneja la recepción y procesamiento de datos del servidor, y actualiza la interfaz de usuario en consecuencia.
///
/// # Errores
///
/// Devuelve un error de tipo `slint::PlatformError` si ocurre un problema con la plataforma.
fn main() -> Result<(), slint::PlatformError> {
    let ui = AppWindow::new()?;
    let ui_clone=ui.as_weak();
    let (tx, rx) = mpsc::channel();

    glog::new().init(Flags::default()).unwrap();

    let conf = Ini::load_from_file("../cliente/settings/cliente.ini").unwrap();

    let section = conf.section(Some("Tcpsettings")).unwrap();
    let direccion = section.get("address").unwrap();
    let puerto = section.get("port").unwrap();
    let segundos = section.get("seconds").unwrap();
    let milisegundos=segundos.to_string();

    let servidor= direccion.to_owned() +":"+puerto;

    info!("Ajustes ini cargados");

    let _tcpthread= thread::Builder::new().name("threadtcp".to_string()).spawn(move||{
        let mut datos_json=String::new();
        loop {
            thread::sleep(Duration::from_millis(milisegundos.clone().parse().unwrap()));
            match TcpStream::connect(servidor.clone().to_string()) {
                Ok(mut stream) => {
                    println!("Successfully connected to server in port 8085");

                    let parsed = json::parse(r#"
                    {
                        "command": "Get playlist"
                    }
                    "#).unwrap();
                    let mensaje=parsed.to_string();
                    let msg=mensaje.as_bytes();

                    stream.write(&*msg).unwrap();
                    println!("Sent Json, awaiting reply...");

                    let mut buf = [0;512];

                    let stmclone = stream.try_clone();
                    match stmclone.unwrap().read(&mut buf){
                        Ok(n) => {
                            let mhh = from_utf8_mut(&mut buf).unwrap().to_string();
                            let my_string= mhh.trim_matches(char::from(0));
                            let myjson= json::parse(my_string).unwrap();
                            datos_json= my_string.parse().unwrap();
                            tx.send(datos_json).unwrap();
                            info!("Estado del servidor + {}",myjson["status"]);

                        },
                        Err(e) => {
                            error!("Failed to receive data: {}", e);
                            println!("Failed to receive data: {}", e);
                            tx.send("{}".parse().unwrap()).unwrap();
                        }
                    }
                },
                Err(e) => {
                    error!("Failed to connect: {}", e);
                    println!("Failed to connect: {}", e);
                    tx.send("{}".parse().unwrap()).unwrap();
                }
            }
            println!("Terminated.");
        }
    }).expect("TODO: panic message");

    let _ui_edit_thread=
        thread::Builder::new().name("ui_edit_thread".to_string()).spawn({move|| {
            for recived in rx{
                let myjson= json::parse(&recived).unwrap();
                if myjson["command"]=="Get playlist" || myjson["status"]=="ok" {

                    let  nombre_c01 = myjson["playlist"][0]["song"].to_string();
                    let  artista_c01 = myjson["playlist"][0]["artist"].to_string();
                    let  guid_c01 = myjson["playlist"][0]["guid"].to_string();

                    let  nombre_c02 = myjson["playlist"][1]["song"].to_string();
                    let  artista_c02 = myjson["playlist"][1]["artist"].to_string();
                    let  guid_c02 = myjson["playlist"][1]["guid"].to_string();

                    let  nombre_c03 = myjson["playlist"][2]["song"].to_string();
                    let  artista_c03 = myjson["playlist"][2]["artist"].to_string();
                    let  guid_c03 = myjson["playlist"][2]["guid"].to_string();

                    let  nombre_c04 = myjson["playlist"][3]["song"].to_string();
                    let  artista_c04 = myjson["playlist"][3]["artist"].to_string();
                    let  guid_c04 = myjson["playlist"][3]["guid"].to_string();

                    let  nombre_c05 = myjson["playlist"][4]["song"].to_string();
                    let  artista_c05 = myjson["playlist"][4]["artist"].to_string();
                    let  guid_c05 = myjson["playlist"][4]["guid"].to_string();

                    let  nombre_c06 = myjson["playlist"][5]["song"].to_string();
                    let artista_c06 = myjson["playlist"][5]["artist"].to_string();
                    let guid_c06 = myjson["playlist"][5]["guid"].to_string();

                    let nombre_c07 = myjson["playlist"][6]["song"].to_string();
                    let artista_c07 = myjson["playlist"][6]["artist"].to_string();
                    let guid_c07 = myjson["playlist"][6]["guid"].to_string();

                    let nombre_c08 = myjson["playlist"][7]["song"].to_string();
                    let artista_c08 = myjson["playlist"][7]["artist"].to_string();
                    let guid_c08 = myjson["playlist"][7]["guid"].to_string();

                    let nombre_c09 = myjson["playlist"][8]["song"].to_string();
                    let artista_c09 = myjson["playlist"][8]["artist"].to_string();
                    let guid_c09 = myjson["playlist"][8]["guid"].to_string();

                    let nombre_c10 = myjson["playlist"][9]["song"].to_string();
                    let artista_c10 = myjson["playlist"][9]["artist"].to_string();
                    let guid_c10 = myjson["playlist"][9]["guid"].to_string();



                    ui_clone.upgrade_in_event_loop(move |ui| ui.set_nombre_c01(nombre_c01.into())).expect("Changed");
                    ui_clone.upgrade_in_event_loop(move |ui| ui.set_artista_c01(artista_c01.into())).expect("Changed");
                    ui_clone.upgrade_in_event_loop(move |ui| ui.set_guid_c01(guid_c01.into())).expect("Changed");

                    ui_clone.upgrade_in_event_loop(move |ui| ui.set_nombre_c02(nombre_c02.into())).expect("Changed");
                    ui_clone.upgrade_in_event_loop(move |ui| ui.set_artista_c02(artista_c02.into())).expect("Changed");
                    ui_clone.upgrade_in_event_loop(move |ui| ui.set_guid_c02(guid_c02.into())).expect("Changed");

                    ui_clone.upgrade_in_event_loop(move |ui| ui.set_nombre_c03(nombre_c03.into())).expect("Changed");
                    ui_clone.upgrade_in_event_loop(move |ui| ui.set_artista_c03(artista_c03.into())).expect("Changed");
                    ui_clone.upgrade_in_event_loop(move |ui| ui.set_guid_c03(guid_c03.into())).expect("Changed");

                    ui_clone.upgrade_in_event_loop(move |ui| ui.set_nombre_c04(nombre_c04.into())).expect("Changed");
                    ui_clone.upgrade_in_event_loop(move |ui| ui.set_artista_c04(artista_c04.into())).expect("Changed");
                    ui_clone.upgrade_in_event_loop(move |ui| ui.set_guid_c04(guid_c04.into())).expect("Changed");

                    ui_clone.upgrade_in_event_loop(move |ui| ui.set_nombre_c05(nombre_c05.into())).expect("Changed");
                    ui_clone.upgrade_in_event_loop(move |ui| ui.set_artista_c05(artista_c05.into())).expect("Changed");
                    ui_clone.upgrade_in_event_loop(move |ui| ui.set_guid_c05(guid_c05.into())).expect("Changed");

                    ui_clone.upgrade_in_event_loop(move |ui| ui.set_nombre_c06(nombre_c06.into())).expect("Changed");
                    ui_clone.upgrade_in_event_loop(move |ui| ui.set_artista_c06(artista_c06.into())).expect("Changed");
                    ui_clone.upgrade_in_event_loop(move |ui| ui.set_guid_c06(guid_c06.into())).expect("Changed");

                    ui_clone.upgrade_in_event_loop(move |ui| ui.set_nombre_c07(nombre_c07.into())).expect("Changed");
                    ui_clone.upgrade_in_event_loop(move |ui| ui.set_artista_c07(artista_c07.into())).expect("Changed");
                    ui_clone.upgrade_in_event_loop(move |ui| ui.set_guid_c07(guid_c07.into())).expect("Changed");

                    ui_clone.upgrade_in_event_loop(move |ui| ui.set_nombre_c08(nombre_c08.into())).expect("Changed");
                    ui_clone.upgrade_in_event_loop(move |ui| ui.set_artista_c08(artista_c08.into())).expect("Changed");
                    ui_clone.upgrade_in_event_loop(move |ui| ui.set_guid_c08(guid_c08.into())).expect("Changed");

                    ui_clone.upgrade_in_event_loop(move |ui| ui.set_nombre_c09(nombre_c09.into())).expect("Changed");
                    ui_clone.upgrade_in_event_loop(move |ui| ui.set_artista_c09(artista_c09.into())).expect("Changed");
                    ui_clone.upgrade_in_event_loop(move |ui| ui.set_guid_c09(guid_c09.into())).expect("Changed");

                    ui_clone.upgrade_in_event_loop(move |ui| ui.set_nombre_c10(nombre_c10.into())).expect("Changed");
                    ui_clone.upgrade_in_event_loop(move |ui| ui.set_artista_c10(artista_c10.into())).expect("Changed");
                    ui_clone.upgrade_in_event_loop(move |ui| ui.set_guid_c10(guid_c10.into())).expect("Changed");

                    ui_clone.upgrade_in_event_loop(move |ui| ui.set_estado("connected".into())).expect("Changed");
                    ui_clone.upgrade_in_event_loop(move |ui| ui.set_visibilidad(false.into())).expect("Changed");

                    println!("Interfaz actualizada");
                    info!("Interfaz actualizada");
                }
                else {
                    ui_clone.upgrade_in_event_loop(move |ui| ui.set_estado("connected".into())).expect("Changed");
                    ui_clone.upgrade_in_event_loop(move |ui| ui.set_visibilidad(true.into())).expect("Changed");

                    println!("Interfaz actualizada a disconnect");
                    info!("Deconectado del servidor, interfaz apagada");
                }
                }
    }}).expect("TODO: panic message");


    ui.on_request_vote_up({
        let ui_handle = ui.as_weak();
        move || {
            println!("btn pressed");

            let ui = ui_handle.unwrap();
            let sguid=ui.get_tempguid().to_string();

            voteup(sguid);
            info!("vote up echo");
        }
    });
    ui.on_request_vote_down({
        let ui_handle = ui.as_weak();
        move || {
            println!("btn pressed");

            let ui = ui_handle.unwrap();
            let sguid=ui.get_tempguid().to_string();

            votedown(sguid);
            info!("vote up echo");
        }
    });

    ui.run()
}

/// Envia un voto negativo (downvote) al servidor para una canción específica identificada por su GUID.
///
/// Esta función lee la configuración del cliente desde un archivo INI, obtiene la dirección y el puerto del servidor TCP
/// al que se va a conectar, y luego establece una conexión TCP con el servidor.
/// Una vez conectado, envía un mensaje JSON al servidor indicando el comando "votedown" y el ID de la canción.
///
/// # Argumentos
///
/// * `song_guid`: El identificador único (GUID) de la canción para la cual se va a enviar el voto negativo.
///
fn votedown(song_guid:String) {
    let conf = Ini::load_from_file("../cliente/settings/cliente.ini").unwrap();
    let section = conf.section(Some("Tcpsettings")).unwrap();
    let direccion = section.get("address").unwrap();
    let puerto = section.get("port").unwrap();
    let servidor= direccion.to_owned() +":"+puerto;

            match TcpStream::connect(servidor.clone().to_string()) {
                Ok(mut stream) => {
                    println!("Successfully connected to server in port 8085");

                    let mut parsed = json::parse(r#"
                    {
                        "command": "votedown",
                        "id": "guid"
                    }
                    "#).unwrap();
                    parsed["id"] = JsonValue::from(song_guid);
                    let mensaje = parsed.to_string();
                    let msg = mensaje.as_bytes();

                    stream.write(&*msg).unwrap();
                }
                _ => {}
            }
            println!("Voto enviado.");
}

/// Envia un voto positivo (upvote) al servidor para una canción específica identificada por su GUID.
///
/// Esta función lee la configuración del cliente desde un archivo INI, obtiene la dirección y el puerto del servidor TCP
/// al que se va a conectar, y luego establece una conexión TCP con el servidor.
/// Una vez conectado, envía un mensaje JSON al servidor indicando el comando "voteup" y el ID de la canción.
///
/// # Argumentos
///
/// * `song_guid`: El identificador único (GUID) de la canción para la cual se va a enviar el voto positivo.
///
fn voteup(song_guid:String) {
    let conf = Ini::load_from_file("../cliente/settings/cliente.ini").unwrap();
    let section = conf.section(Some("Tcpsettings")).unwrap();
    let direccion = section.get("address").unwrap();
    let puerto = section.get("port").unwrap();
    let servidor= direccion.to_owned() +":"+puerto;

    match TcpStream::connect(servidor.clone().to_string()) {
        Ok(mut stream) => {
            println!("Successfully connected to server in port 8085");

            let mut parsed = json::parse(r#"
                    {
                        "command": "voteup",
                        "id": "guid"
                    }
                    "#).unwrap();
            parsed["id"] = JsonValue::from(song_guid);
            let mensaje = parsed.to_string();
            let msg = mensaje.as_bytes();

            stream.write(&*msg).unwrap();
        }
        _ => {}
    }
    println!("Voto enviado.");
}
