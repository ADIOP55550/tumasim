
<script lang="ts">
  import "milligram/dist/milligram.min.css";
  import TMSim from "$lib/TM/TMSim.svelte"

  let codeElement;

  const getCaretPosition = (elementRef) => {
    var selection = document.getSelection();
    if (!selection || !elementRef) return 0;
    selection.collapseToEnd();
    const range = selection.getRangeAt(0);
    const clone = range.cloneRange();
    clone.selectNodeContents(elementRef);
    clone.setEnd(range.startContainer, range.startOffset);
    return clone.toString().length;
  }

  const setCaretPosition = (elem, caretPos) => {

      if(elem != null) {
          if(elem.createTextRange) {
              var range = elem.createTextRange();
              range.move('character', caretPos);
              range.select();
          }
          else {
              if(elem.selectionStart) {
                  elem.focus();
                  elem.setSelectionRange(caretPos, caretPos);
              }
              else
                  elem.focus();
          }
      }
  }
  
  function handleKeyDown(event: KeyboardEvent){
    // console.dir(event);
    switch(event.key) {
      case "Tab":
        {
          event.preventDefault();
          const caretPos = getCaretPosition(codeElement);
          console.dir(codeElement.textContent.valueOf())
          codeElement.textContent = 
            codeElement.textContent.substring(0, caretPos)
            + '\t'
            + codeElement.textContent.substring(caretPos);

          setTimeout(()=>{
            setCaretPos(codeElement, caretPos + 1);
          }, 4)
        }
    }
  }
</script>

<h1>TuMaSim</h1>

<form>
  <fieldset>
    <pre><code contenteditable=true on:keydown={handleKeyDown} bind:this={codeElement}></code></pre>
    
    <label for="initialState">Initial state:</label>
    <input type="text" placeholder="A" id="initialState" pattern="\w+">

    <label for="acceptingStates">Accepting states:</label>
    <input type="text" placeholder="B, C, D" id="acceptingStates">

    <button class="button">Load</button>
</fieldset>
</form>

<TMSim/>



