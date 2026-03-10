#!/bin/bash
usr="/home/freddskiii/cloud_storage/cloud_log.txt"
picDir="/home/freddskiii/cloud_storage/downloads"
user=$(awk -F";" '/userLogged/ {print $2}' $usr)

if [[ $user ]]; then
    if ! [[ -d "$picDir/$user" ]]; then
    mkdir -p "$picDir/$user"
    fi

    pic=(
        "https://onetreeplanted.org/cdn/shop/articles/nature_facts_1600x.jpg?v=1705008496"
        "https://natureinvestmenthub.ca/wp-content/uploads/2023/10/hendrik-cornelissen-qrcOR33ErA-unsplash-scaled.jpg"
        "https://i0.wp.com/picjumbo.com/wp-content/uploads/beautiful-fall-nature-scenery-free-image.jpeg?w=600&quality=80"
        "https://natureconservancy-h.assetsadobe.com/is/image/content/dam/tnc/nature/en/photos/w/o/WOPA160517_D056-resized.jpg?crop=864%2C0%2C1728%2C2304&wid=600&hei=800&scl=2.88"
        "https://deframedia.blog.gov.uk/wp-content/uploads/sites/208/2023/09/State-of-nature.jpg"
        "https://img-4.linternaute.com/0efC7-fKD0qAHCUcChtSVXT9Ego=/1500x/smart/43f787f117784c3fa9a928194bd91d5e/ccmcms-linternaute/12325430.jpg"
        "https://scx2.b-cdn.net/gfx/news/hires/2020/nature.jpg"
        "https://img.freepik.com/premium-photo/wide-angle-shot-single-tree-growing-clouded-sky-sunset-surrounded-by-grass_181624-22807.jpg"
        "https://www.aaronreedphotography.com/images/xl/Sweet-Dreams-2022.jpg"
        "https://upload.wikimedia.org/wikipedia/commons/thumb/8/83/Waterfall_at_Arthur%27s_Pass_National_Park.jpg/800px-Waterfall_at_Arthur%27s_Pass_National_Park.jpg"
        "https://www.tyrol.com/images/gtgX0XtnCuQ/cb:/crop:2000:2997:nowe:0:0/gravity:fp:0.5:0.5/aHR0cHM6Ly93d3cudHlyb2wuY29tL2ZpbGVhZG1pbi90aXJvbC9XYW5kZXJuL1dhc3NlcmYlQzMlQTRsbGUvU3R1aWJlbmZhbGwtb2V0enRhbC10b3VyaXNtdXMuanBn"
        "https://www.dereknielsen.com/wp-content/uploads/2023/11/edge-of-autumn.jpg"
        "https://images.newscientist.com/wp-content/uploads/2023/02/07104439/SEI_142739270.jpg"
        "https://tnfd.global/wp-content/uploads/2023/09/Food-and-Agricultulre-cover-1.jpg"
        "https://www.andrewshoemaker.com/images/xl/revive-waianapanapa-state-park.jpg"
    )

    filename="$(date +'%H-%M_%d-%m-%Y').jpg"
    random_index=$((RANDOM % ${#pic[@]}))
    wget -q "${pic[$random_index]}" -O "$picDir/$user/$filename"
fi 
