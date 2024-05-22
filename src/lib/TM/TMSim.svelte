
<script>
  let pointerPos = $state(5);
  let pointerElem;

  $effect(()=>{
    console.log(pointerPos);

    pointerElem.scrollIntoView()
  });
  
</script>

<div class="button" on:click={()=>{pointerPos--}}>-</div>
<div class="button" on:click={()=>{pointerPos++}}>+</div>
<div id="tape">
  <div class="cell" data-pos="0">_</div>
  <div class="cell" data-pos="1">_</div>
  <div class="cell" data-pos="2">_</div>
  <div class="cell" data-pos="3">_</div>
  <div class="cell" data-pos="4">_</div>
  <div class="cell" data-pos="5">_</div>
  <div class="cell" data-pos="6">_</div>
  <div class="cell" data-pos="7">_</div>
  <div class="cell" data-pos="8">_</div>
  <div class="cell" data-pos="9">_</div>
  <div class="cell" data-pos="10">_</div>
  <div class="cell" data-pos="11">_</div>
  <div class="cell" data-pos="12">_</div>
  <div class="cell" data-pos="13">_</div>
  <div class="cell" data-pos="14">_</div>
  <div class="cell" data-pos="15">_</div>
  <div class="cell" data-pos="16">_</div>
  <div class="cell" data-pos="17">_</div>
  <div class="cell" data-pos="18">_</div>
  <div class="cell" data-pos="19">_</div>
  <div class="cell" data-pos="20">_</div>
  <div class="cell" data-pos="21">_</div>
  <div class="cell" data-pos="22">_</div>
  <div class="cell" data-pos="23">_</div>
  <div class="cell" data-pos="24">_</div>
  <div class="cell" data-pos="25">_</div>
  <div class="cell" data-pos="26">_</div>
  <div class="cell" data-pos="27">_</div>
  <div class="cell" data-pos="28">_</div>
  <div class="cell" data-pos="29">_</div>
  <div class="cell" data-pos="30">_</div>
  <div class="cell" data-pos="31">_</div>
  <div class="cell" data-pos="32">_</div>
  <div class="cell" data-pos="33">_</div>
  <div class="cell" data-pos="34">_</div>
  <div class="cell" data-pos="35">_</div>
  <div class="cell" data-pos="36">_</div>
  <div class="cell" data-pos="37">_</div>
  <div class="cell" data-pos="38">_</div>
  <div class="cell" data-pos="39">_</div>
  <div class="cell" data-pos="40">_</div>
  <div class="cell" data-pos="41">_</div>
  <div class="cell" data-pos="42">_</div>
  <div class="cell" data-pos="43">_</div>
  <div class="cell" data-pos="44">_</div>
  <div class="pointer-wrapper">
    <div id="pointer" bind:this={pointerElem} style:grid-column={pointerPos + ' / ' + (pointerPos + 1)}></div>
  </div>
</div>


<style>
  #tape {
    display: grid;
    grid-template-columns: repeat(45, 1fr);
    grid-template-rows: 1fr 1fr;
    width: 100%;
    gap: .1em;
    overflow-x: scroll;
  }
  .cell {
    border: 2px solid black;
    width: 2em;
    height: 2em;
    vertical-align: middle;
    text-align: center;
  }
  .pointer-wrapper {
    display: grid;
    grid-template-columns: subgrid;
    grid-row: 2 / 3;
    grid-column: 1 / -1;
  }
  #pointer {
    display: block;
    border-top: 5px solid black;
    border-right: 5px solid black;
    transform: rotateZ(-45deg) scale(0.3);
  }
</style>
