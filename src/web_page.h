#pragma once

#include <Arduino.h>

const char INDEX_HTML[] PROGMEM = R"HTML(<!doctype html>
<html lang="ja">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width,initial-scale=1">
<meta name="theme-color" content="#090b12">
<title>ATOM Matrix Pixel Studio</title>
<style>
:root{color-scheme:dark;font-family:Inter,"Noto Sans JP",system-ui,sans-serif;--bg:#090b12;--panel:#121622;--line:#2a3041;--ink:#f5f7ff;--muted:#8992a8;--cyan:#62e6ff;--pink:#ff5c8a;--yellow:#ffd166}
*{box-sizing:border-box}
body{min-height:100vh;margin:0;display:grid;place-items:center;padding:34px 18px;color:var(--ink);background:radial-gradient(circle at 12% 18%,#173349 0,transparent 29%),radial-gradient(circle at 88% 82%,#371933 0,transparent 31%),var(--bg)}
body:before{position:fixed;inset:0;pointer-events:none;content:"";background-image:linear-gradient(#ffffff08 1px,transparent 1px),linear-gradient(90deg,#ffffff08 1px,transparent 1px);background-size:24px 24px;mask-image:linear-gradient(#0007,transparent 88%)}
main{position:relative;width:min(100%,940px);overflow:hidden;border:1px solid #ffffff18;border-radius:28px;background:#0e111bdd;box-shadow:0 28px 90px #000b;backdrop-filter:blur(14px)}
header{display:flex;justify-content:space-between;gap:18px;align-items:center;padding:24px 27px;border-bottom:1px solid var(--line)}
.eyebrow{margin:0 0 5px;color:var(--cyan);font:800 .68rem/1 ui-monospace,monospace;letter-spacing:.2em}
h1{margin:0;font-size:clamp(1.35rem,4vw,2rem);letter-spacing:-.03em}.subtitle{margin:7px 0 0;color:var(--muted);font-size:.82rem}
.badge{display:grid;place-items:center;min-width:78px;aspect-ratio:1;border:1px solid #62e6ff55;border-radius:18px;color:var(--cyan);background:#62e6ff0b;box-shadow:inset 0 0 22px #62e6ff10}
.badge strong{font:900 1.5rem/1 ui-monospace,monospace}.badge span{margin-top:-13px;font-size:.57rem;letter-spacing:.13em}
.workspace{display:grid;grid-template-columns:minmax(340px,1.05fr) minmax(300px,.95fr)}
.stage{display:grid;place-items:center;min-height:560px;padding:38px;border-right:1px solid var(--line);background:radial-gradient(circle,#20283b 0,#111520 66%)}
.device{width:min(100%,390px);padding:24px;border:1px solid #ffffff1a;border-radius:38px;background:linear-gradient(145deg,#252a35,#10131a);box-shadow:inset 2px 2px 3px #ffffff17,inset -5px -5px 10px #0008,0 28px 45px #0009}
.device-top{display:flex;justify-content:space-between;margin:0 5px 17px;color:#727b90;font:700 .61rem ui-monospace,monospace;letter-spacing:.13em}
.matrix{display:grid;grid-template-columns:repeat(5,1fr);gap:9px;padding:17px;border:1px solid #ffffff12;border-radius:27px;background:#05070b;box-shadow:inset 0 0 24px #000}
.pixel{position:relative;aspect-ratio:1;padding:0;border:1px solid #ffffff19;border-radius:14px;background:#050608;box-shadow:inset 0 0 0 5px #090b10;cursor:crosshair;touch-action:none;transition:transform .08s,border-color .12s,box-shadow .12s}
.pixel:hover{transform:scale(1.04);border-color:#ffffff55}.pixel.selected{outline:2px solid var(--cyan);outline-offset:2px;z-index:1}
.pixel span{position:absolute;right:8px;bottom:6px;color:#ffffff99;font:700 .54rem ui-monospace,monospace;text-shadow:0 1px 3px #000}
.device-bottom{display:flex;align-items:center;justify-content:space-between;margin:18px 7px 1px;color:#697287;font:700 .6rem ui-monospace,monospace;letter-spacing:.12em}.port{width:52px;height:10px;border-radius:7px;background:#06070b;box-shadow:inset 0 2px 4px #000}
.controls{padding:29px 30px 31px;background:linear-gradient(155deg,#151927,#0d1019)}
.section-title{display:flex;justify-content:space-between;align-items:center;margin-bottom:15px}.section-title h2{margin:0;font-size:.82rem;letter-spacing:.1em}.safe{color:#83edb0;font:700 .62rem ui-monospace,monospace}
.picker-row{display:grid;grid-template-columns:78px 1fr;gap:14px;align-items:center}
input[type=color]{width:78px;height:78px;padding:5px;border:1px solid #ffffff24;border-radius:18px;background:#080a10;cursor:pointer}
label{display:block;margin:0 0 6px;color:var(--muted);font-size:.68rem;letter-spacing:.08em}
input[type=text]{width:100%;padding:13px;border:1px solid var(--line);border-radius:12px;outline:0;color:var(--ink);background:#090b12;font:800 .93rem ui-monospace,monospace;text-transform:uppercase}input[type=text]:focus{border-color:var(--cyan)}
#selected{margin:7px 0 0;color:#6f788d;font:600 .68rem ui-monospace,monospace}
.swatches{display:grid;grid-template-columns:repeat(7,1fr);gap:8px;margin:18px 0 22px}.swatch{aspect-ratio:1;border:2px solid #ffffff2b;border-radius:50%;background:var(--color);cursor:pointer}.swatch:hover,.swatch:focus-visible{transform:scale(1.1);outline:2px solid white;outline-offset:2px}
.tips{margin:0 0 21px;padding:12px 14px;border:1px solid #62e6ff20;border-radius:13px;color:#8996ab;background:#62e6ff07;font-size:.7rem;line-height:1.65}.tips b{color:var(--cyan)}
.minor-actions{display:grid;grid-template-columns:1fr 1fr;gap:9px}.minor-actions button,.save{border-radius:12px;font-weight:800;cursor:pointer}
.minor-actions button{padding:11px 6px;border:1px solid var(--line);color:#bec5d5;background:#0b0e16}.minor-actions button:hover{border-color:#5e687e}
.save{width:100%;margin-top:10px;padding:14px;border:0;color:#071116;background:linear-gradient(90deg,var(--cyan),#9cf5dc);font-size:.9rem;box-shadow:0 10px 26px #62e6ff18}.save:hover{filter:brightness(1.08)}button:disabled{cursor:wait;opacity:.55}
#status{min-height:1.4em;margin:13px 2px 0;color:#9aa4b8;text-align:center;font-size:.7rem}#status.ok{color:#83edb0}#status.bad{color:#ff8199}#status.dirty{color:var(--yellow)}
@media(max-width:760px){body{padding:14px}header{padding:19px}.workspace{grid-template-columns:1fr}.stage{min-height:auto;padding:26px 24px;border-right:0;border-bottom:1px solid var(--line)}.device{max-width:360px}.controls{padding:25px 22px}}
@media(max-width:390px){.badge{display:none}.stage{padding:20px 14px}.device{padding:17px;border-radius:28px}.matrix{gap:7px;padding:12px}.pixel{border-radius:11px}.controls{padding:22px 17px}}
</style>
</head>
<body>
<main>
  <header>
    <div><p class="eyebrow">PIXEL CONTROL / 5×5</p><h1>ATOM Matrix Pixel Studio</h1><p class="subtitle">25個のLEDを、ひとつずつ自由にペイント。</p></div>
    <div class="badge"><strong>25</strong><span>RGB LED</span></div>
  </header>
  <div class="workspace">
    <section class="stage" aria-label="LEDマトリクス編集エリア">
      <div class="device">
        <div class="device-top"><span>ATOM MATRIX</span><span>FRONT</span></div>
        <div id="matrix" class="matrix" aria-label="5×5 LEDマトリクス"></div>
        <div class="device-bottom"><span>M5STACK</span><i class="port" aria-hidden="true"></i><span>USB-C</span></div>
      </div>
    </section>
    <section class="controls">
      <div class="section-title"><h2>PAINT COLOR</h2><span class="safe">● SAFE 20 / 255</span></div>
      <div class="picker-row">
        <input id="picker" type="color" value="#62e6ff" aria-label="ペイント色">
        <div><label for="hex">カラーコード</label><input id="hex" type="text" value="#62E6FF" maxlength="7" spellcheck="false"><p id="selected">LED 01 · ROW 1 / COL 1</p></div>
      </div>
      <div id="swatches" class="swatches" aria-label="カラープリセット"></div>
      <p class="tips"><b>操作：</b>色を選び、LEDをクリックまたはドラッグして塗ります。編集後に「ATOM Matrixへ反映」を押してごらん。</p>
      <div class="minor-actions"><button id="off" type="button">すべて消灯</button><button id="reload" type="button">本体から再読込</button></div>
      <button id="save" class="save" type="button">ATOM Matrixへ反映</button>
      <p id="status" role="status" aria-live="polite">LEDデータを読み込んでいます…</p>
    </section>
  </div>
</main>
<script>
const COUNT=25,DEFAULT=['#FF4D6D','#000000','#000000','#000000','#4DABFF','#FF4D6D','#FF9F43','#000000','#7C5CFC','#4DABFF','#FF4D6D','#000000','#FFE66D','#000000','#4DABFF','#FF4D6D','#000000','#000000','#000000','#4DABFF','#FF4D6D','#000000','#000000','#000000','#4DABFF'],PRESETS=['#62E6FF','#FF5C8A','#FFD166','#83EDB0','#7C5CFC','#FFFFFF','#000000'];
const matrix=document.querySelector('#matrix'),picker=document.querySelector('#picker'),hex=document.querySelector('#hex'),selected=document.querySelector('#selected'),swatches=document.querySelector('#swatches'),off=document.querySelector('#off'),reload=document.querySelector('#reload'),save=document.querySelector('#save'),status=document.querySelector('#status');
let colors=[...DEFAULT],brush=picker.value.toUpperCase(),active=0,painting=false;
const valid=v=>/^#[0-9a-f]{6}$/i.test(v);
function say(text,kind=''){status.textContent=text;status.className=kind}
function label(i){return `LED ${String(i+1).padStart(2,'0')} · ROW ${Math.floor(i/5)+1} / COL ${i%5+1}`}
function renderPixel(i){const p=matrix.children[i],c=colors[i];p.style.background=c;p.style.boxShadow=c==='#000000'?'inset 0 0 0 5px #090b10':`inset 0 0 0 5px #090b10,0 0 17px ${c}99`;p.classList.toggle('selected',i===active);p.title=`${label(i)} · ${c}`;p.setAttribute('aria-label',p.title)}
function renderAll(){colors.forEach((_,i)=>renderPixel(i));selected.textContent=label(active)}
function setBrush(value){if(!valid(value))return;brush=value.toUpperCase();picker.value=brush;hex.value=brush}
function paint(i){active=i;colors[i]=brush;renderAll();say('未送信の変更があります。','dirty')}
for(let i=0;i<COUNT;i++){const p=document.createElement('button');p.type='button';p.className='pixel';p.dataset.index=i;p.innerHTML=`<span>${String(i+1).padStart(2,'0')}</span>`;matrix.appendChild(p)}
PRESETS.forEach(c=>{const b=document.createElement('button');b.type='button';b.className='swatch';b.style.setProperty('--color',c);b.title=c;b.setAttribute('aria-label',c);b.addEventListener('click',()=>setBrush(c));swatches.appendChild(b)});
matrix.addEventListener('pointerdown',e=>{const p=e.target.closest('.pixel');if(!p)return;e.preventDefault();painting=true;paint(Number(p.dataset.index))});
matrix.addEventListener('pointerover',e=>{const p=e.target.closest('.pixel');if(p&&painting)paint(Number(p.dataset.index))});
window.addEventListener('pointerup',()=>painting=false);window.addEventListener('pointercancel',()=>painting=false);
picker.addEventListener('input',()=>setBrush(picker.value));hex.addEventListener('input',()=>{const v=hex.value.trim();if(valid(v))setBrush(v)});hex.addEventListener('blur',()=>hex.value=brush);
off.addEventListener('click',()=>{colors.fill('#000000');renderAll();say('全消灯をプレビュー中です。反映ボタンを押してください。','dirty')});
async function loadColors(){reload.disabled=true;say('LEDデータを読み込んでいます…');try{const r=await fetch('/api/leds',{cache:'no-store'});const d=await r.json();if(!r.ok||!Array.isArray(d.colors)||d.colors.length!==COUNT)throw Error();colors=d.colors.map(c=>String(c).toUpperCase());renderAll();say('25個のLEDデータを読み込みました。','ok')}catch(e){colors=[...DEFAULT];renderAll();say('ATOM Matrixと通信できません。プレビューを編集できます。','bad')}finally{reload.disabled=false}}
reload.addEventListener('click',loadColors);
save.addEventListener('click',async()=>{save.disabled=true;say('ATOM Matrixへ送信しています…');try{const body=new URLSearchParams({colors:colors.join(',')});const r=await fetch('/api/leds',{method:'POST',body});const d=await r.json();if(!r.ok)throw Error(d.error||'apply failed');colors=d.colors.map(c=>String(c).toUpperCase());renderAll();say('25個のLEDへ反映しました。リセットすると初期値へ戻ります。','ok')}catch(e){say('送信できませんでした。接続を確認してください。','bad')}finally{save.disabled=false}});
renderAll();loadColors();
</script>
</body>
</html>)HTML";
