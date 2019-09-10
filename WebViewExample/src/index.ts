import "./styles/app.scss";
import "./styles/button.scss";
import "./styles/container.scss";
import "./styles/list.scss";

function onAddClick(e: MouseEvent) {
    const list = document.getElementById('list');
    const index = (list.childElementCount + 1);
    const x = 0;
    const y = 0;

    const item = document.createElement('div');
    item.classList.add('item');
    item.classList.add('container');
    const title = document.createElement('div');
    title.classList.add('title');
    title.innerText = 'Item ' + index;
    item.appendChild(title);
    const content = document.createElement('div');
    content.classList.add('content');

    if (index % 2 === 0) {
        content.style.backgroundImage =
        `url(https://edgetipscdn.microsoft.com/insider-site/images/logo-dev.c8d75c3b.png)`;
    } else {
        content.style.backgroundImage =
        `url(https://edgetipscdn.microsoft.com/insider-site/images/logo-canary.a897af1f.png)`;
    }

    item.appendChild(content);
    list.appendChild(item);
}

// Add the event handler as soon as the script loads
document.getElementById('addButton').addEventListener('click', onAddClick);

// Fill in some items for testing
for (let i = 0; i < 2; i++) {
    onAddClick(null);
}
