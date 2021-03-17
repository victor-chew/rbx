let filelist = [];
let autorun = '';

ace.config.set('basePath', 'https://cdnjs.cloudflare.com/ajax/libs/ace/1.4.12'); 
let editor = ace.edit('editor');
editor.setShowPrintMargin(false);
editor.session.setMode('ace/mode/javascript');

let new_prog_btn = document.getElementById('new_prog');
let open_prog_btn = document.getElementById('open_prog');
let save_prog_btn = document.getElementById('save_prog');
let run_prog_btn = document.getElementById('run_prog');
let stop_prog_btn = document.getElementById('stop_prog');
let download_backup_btn = document.getElementById('download_backup');
let restore_backup_btn = document.getElementById('restore_backup');
let logger = document.getElementById('logger');
let prog_name = document.getElementById('prog_name');
let buttons_set1 = [new_prog_btn, open_prog_btn, save_prog_btn, download_backup_btn, restore_backup_btn];
let buttons_set2 = buttons_set1.concat([run_prog_btn]);

let new_dialog = document.getElementById('new_dialog');
document.getElementById('confirm_new').addEventListener('click', e => {
	e.preventDefault();
	new_prog();
	new_dialog.close();
});

let open_dialog = document.getElementById('open_dialog');
let open_prog_list = document.getElementById('open_prog_list');
document.getElementById('confirm_open').addEventListener('click', e => {
	e.preventDefault();
	open_dialog.close();
	open_prog(open_prog_list.value);
});
document.getElementById('delete_prog').addEventListener('click', e => {
	e.preventDefault();
	delete_prog(open_prog_list.value, open_prog_list.selectedIndex);
});

let save_dialog = document.getElementById('save_dialog');
document.getElementById('confirm_save').addEventListener('click', e => {
	e.preventDefault();
	let oldName = prog_name.innerText;
	let newName = sanitize_filename(document.getElementById('save_name').value);
  prog_name.innerText = newName;
	idbKeyval.set('scriptName', newName);
	idbKeyval.set('scriptText', editor.getValue());
	save_prog();
	save_dialog.close();
});

// Autosave every 5s
setInterval(() => { 
  let script = editor.getValue().trim();
  idbKeyval.set('scriptName', prog_name.innerText);
	if (script.length > 0) idbKeyval.set('scriptText', script); 
}, 5*1000); 

idbKeyval.get('scriptName').then(val => {
	if (val !== undefined) {
		prog_name.innerText = val;
		idbKeyval.get('scriptText').then(val => { if (val !== undefined) set_prog(val); else new_prog(); });
	} else {
    new_prog();
	}
});

let server = new EventSource("events");
server.onerror = e => append_log('<font color="red"><b>*** CONNECTION ERROR</b></font>');
server.onmessage = e => {
  console.log(e.data);
	let type = e.data[0];
	let msg = e.data.substring(2);
	switch(type) {
		case 'S': 
			msg = '<font color="green"><b>' + msg + '</b></font>'; 
			append_log(msg); 
			break;
		case 'E': 
      let lines = msg.split('\n');
      lines = lines.filter(line => line.indexOf('native ') === -1);
      lines = lines.filter(line => line.indexOf('[anon] ') === -1);
      lines = lines.map(line => line.replace(/preventsyield/, ' '));
      lines = lines.map(line => line.replace(/\n/g, '<br />'));
      lines = lines.map(line => line.replace(/\(line (\d+)([^)]*?)\)/g,
        '<button class="line_num" onclick="goto_line($1)">$1</button>$2'));
      lines = lines.map(line => line.replace(/\(\:(\d+)\)/g,
        '<button class="line_num" onclick="goto_line($1)">$1</button>'));
      lines[0] = lines[0].trim();
      for (let i=1; i<lines.length; i++) lines[i] = '...' + lines[i].trim();
      append_log('<font color="red"><b>' + lines.join('<br />') + '</b></font>');
			break;
		case 'C': 
			process_event(msg); 
			break;
		case 'F':
      enable_interaction(buttons_set2, true);
      if (msg === '/rbx') {
        filelist = [];
        autorun = '';
      }
      else {
        let scriptName = msg;
        if (scriptName.charAt(0) == '*') {
          scriptName = scriptName.substring(1);
          autorun = scriptName;
        }
        filelist.push(scriptName);
      }
			break;
		default:
			append_log(msg);
	}
}

let splitter = Split(["#editor", "#logger_wrapper"], { 
	sizes: [75, 25],
	direction: 'vertical',
	minSize: 0,
	gutterSize: 3,
});

window.addEventListener('keydown', e => {
  // During execution, relay keystrokes to controller
  if (run_prog_btn.style.display == 'none') {
    let result = e.code;
    if (e.ctrlKey) result += '+Ctrl';
    if (e.altKey) result += '+Alt';
    if (e.shiftKey) result += '+Shift';
    send_key(result);
    e.preventDefault();
  } 
  // During editing, block Ctrl-S from reaching Chrome, which will cause a connection error when the save page dialog pops up.
  else if (!e.altKey	&& !e.shiftKey && e.ctrlKey && e.code == 'KeyS') {
      e.preventDefault();
  }
});

let fileupload = document.getElementById('fileupload');
fileupload.addEventListener('change', () => {
  if (fileupload.files.length == 1) {
    enable_interaction(buttons_set2, false);
    let xhr = new XMLHttpRequest();
    let fd = new FormData();
    xhr.open("POST", "/restore", true);
    fd.append('tarfile', fileupload.files[0]);
    xhr.send(fd);
  }
});

// ---------------------- END MAIN -------------------------

function goto_line(num) {
  editor.resize(true);
	editor.focus();
  editor.scrollToLine(num, true, true, () => {});
	editor.gotoLine(num, 0, false);
}

function sanitize_filename(str) {
  str = str.replace(/^\./, '');
  str = str.replace('/', '_');
  return str.substring(0, 31-5); // max length of 26 chars
}

function process_event(e) {
	switch(e) {
    case 'RUNNING':
		case 'START':
			run_prog_btn.style.display = 'none';
      stop_prog_btn.style.display = 'inline-block';
      stop_prog_btn.onclick = stop_prog;
      editor.setOptions({ readOnly: true });
      editor.container.style.pointerEvents = 'none';
      editor.container.style.opacity = 0.5;
      editor.renderer.setStyle("disabled", true);
      enable_interaction(buttons_set1, false);
      break;
    case 'READY':
      append_log('<font color="green"><b>Ready</b></font>');
      // Fall-through
    case 'STOP': 
			stop_prog_btn.style.display = 'none';
			run_prog_btn.style.display = 'inline-block';
      run_prog_btn.onclick = run_prog;
      editor.setOptions({ readOnly: false });
      editor.container.style.pointerEvents = 'auto';
      editor.container.style.opacity = 1.0;
      editor.renderer.setStyle("disabled", false);
      enable_interaction(buttons_set1, true);
			break;
	}
}

function enable_interaction(buttons, enabled) {
  for (let i=0; i<buttons.length; i++) {
    buttons[i].style.pointerEvents = (enabled ? 'auto' : 'none');
    buttons[i].style.opacity = (enabled ? 1.0 : 0.2);
  }
}

function show_open_dialog() {
	let select = document.getElementById('open_prog_list');
  for (let i=select.options.length-1; i>=0; i--) select.remove(i);
  filelist.sort();
	filelist.forEach((name) => {
    let opt = document.createElement('option');
		opt.appendChild(document.createTextNode(name));
		opt.value = name;
    if (name === autorun) {
      opt.style.fontWeight = 'bold';
      opt.style.color = '#ff0000';
    }
		select.appendChild(opt);
	});
	open_dialog.showModal();
}

function show_save_dialog() {
  document.getElementById('save_name').value = prog_name.innerText;
  document.getElementById('save_autorun').checked = (prog_name.innerText === autorun);
	save_dialog.showModal();
}

function new_prog() {
	let name = 'RBX Script ';
	for (i=1; i<=Number.MAX_SAFE_INTEGER; i++) {
		let tmpname = name + i;
		if (filelist.indexOf(tmpname) == -1) {
			name = tmpname;
			break;
		}
	}
	prog_name.innerText = name;
  set_prog("print('Hello World!');");
}

function get_prog_list() {
	let xhr = new XMLHttpRequest();
	xhr.open("GET", "/list", true);
	xhr.send();
}

function delete_prog(name, index) {
	let xhr = new XMLHttpRequest();
	xhr.onreadystatechange = () => {
    if (xhr.readyState === XMLHttpRequest.DONE && xhr.status === 200) {
			open_prog_list.remove(index);
		}
	}
	xhr.open("POST", "/delete", true);
	xhr.setRequestHeader('Content-Type', 'application/json');
	xhr.send(JSON.stringify({ 'name': name }));
}

function set_prog(text) {
  editor.setValue(text);
  goto_line(1);
}

function open_prog(name) {
	let xhr = new XMLHttpRequest();
	xhr.onreadystatechange = () => {
    if (xhr.readyState === XMLHttpRequest.DONE && xhr.status === 200) {
			prog_name.innerText = name;
			set_prog(xhr.responseText);
		}
	}
	xhr.open("POST", "/open", true);
	xhr.setRequestHeader('Content-Type', 'application/json');
	xhr.send(JSON.stringify({ 'name': name }));
}	

function save_prog() {
  enable_interaction(buttons_set2, false);
	let xhr = new XMLHttpRequest();
	xhr.open("POST", "/save", true);
	xhr.setRequestHeader('Content-Type', 'application/json');
	xhr.send(JSON.stringify({ 
		'name': prog_name.innerText, 
    'script': editor.getValue(),
    'autorun': document.getElementById('save_autorun').checked,
	}));
}

function run_prog() {
  run_prog_btn.onclick = null;
  let xhr = new XMLHttpRequest();
	xhr.open("POST", "/run", true);
  xhr.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded');
  xhr.send(encodeURIComponent('script') + '=' + encodeURIComponent(editor.getValue()));
}

function stop_prog() {
  stop_prog_btn.onclick = null;
	let xhr = new XMLHttpRequest();
	xhr.open("GET", "/stop", true);
	xhr.send();
}

function send_key(key) {
	let xhr = new XMLHttpRequest();
	xhr.open("POST", "/key", true);
	xhr.setRequestHeader('Content-Type', 'application/json');
	xhr.send(JSON.stringify({ 
		'key': key,
	}));
}

function restore_backup() {
  fileupload.value = '';
  fileupload.click();
}

function append_log(line) {
	logger.innerHTML += line + '<br />';
	let container = document.getElementById('logger_wrapper');
	container.scrollTop = logger.scrollHeight - container.clientHeight;
}

function clear_log() {
	logger.innerHTML = '';
}

function name_check(element)
{
	let maxlen = 31-5;
	let name = element.value;
	name = name.replace('/', '');
	if (name.length > maxlen) name = name.substring(0, maxlen);
	element.value = name;
}
